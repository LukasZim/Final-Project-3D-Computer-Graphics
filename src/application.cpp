// #include "Image.h"
#include "mesh.h"
#include "texture.h"
// Always include window first (because it includes glfw, which includes GL
// which needs to be included AFTER glew). Can't wait for modules to fix this
// stuff...
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glad/glad.h>
// Include glad before glfw3
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
DISABLE_WARNINGS_POP()
#include <framework/shader.h>
#include <framework/window.h>

#include <functional>
#include <iostream>
#include <vector>
#include "powerup.cpp"
#include "player.cpp"
#include <glm/gtx/string_cast.hpp>
//#include "enemy.cpp"
#include "environment.cpp"
#include "snake.cpp"
#include "planet.cpp"
#include "robot.cpp"

class Application {
  public:
	  int startMouseX = 0;
	  int startMouseY = 0;

	  bool mouse0Held = false;
	  bool goingForwards = false;
	  bool goingRight = false;
	  bool goingLeft = false;
	  bool goingBackwards = false;
	  bool topviewEnabled = false;
	  bool shooting = false;

	  int endMouseX = 0;
	  int endMouseY = 0;
	  int framecounter = 0;




	Application()
		: 
		m_window("Final Project", glm::ivec2(1024, 1024), OpenGLVersion::GL45),
		powerup1("resources/cube-textured.obj", "resources/default.png", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3(10.0f, 10.0f, 10.0f)), glm::vec3(5, 0, 5))),
		enemy1("resources/enemyship/UFO.obj",
			"resources/enemyship/reinforced-metal_metallic.png", 
			glm::mat4{ 1.0f }, 
			2000, 
			{ 8.0f, 74.0f, 81.0f, 11.0f } ,
			{ 130.0f, 230.0f, 330.0f, 430.0f }
		),
		enemy2("resources/enemyship/UFO.obj",
			"resources/enemyship/reinforced-metal_metallic.png", 
			glm::mat4{1.0f},
			1881,
			{0.0f, 100.0f, 100.0f, 0.0f},
			{0.0f, 0.0f, 100.0f, 100.0f}
			),
		enemy3("resources/enemyship/UFO.obj",
			"resources/enemyship/reinforced-metal_metallic.png",
			glm::mat4{ 1.0f },
			1914, 
			{-100.0f, -50.0f, -25.0f, -100.0f},
			{-23, -98, -11, -11}
			),
		robot(glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 20.0 }), glm::vec3(-10, 0, 0))),
		planets("resources/ceres/ceres.obj", "resources/ficunatus/ficunatus.obj", "resources/jupiter/jupiter.obj", "resources/venus/venus.obj", //4 objs
			"resources/ceres/ceres.jpg", "resources/ficunatus/ficunatus.jpg", "resources/jupiter/jupiter.jpg", "resources/venus/venus.jpg",      //4 textures    
			glm::mat4(1.0f)),
		snake("resources/snake/snake1.obj", "resources/snake/snake2.obj", "resources/snake/snake3.obj", "resources/snake/pure_blue.png", glm::scale(glm::mat4{ 1.0 }, glm::vec3{50.0f})),
		player("resources/Gunship_model/space-cruiser-panels2_normal-ogl.png", glm::mat4{ 1.0 }),
		secondaryLight(glm::vec3(0, 10, 0), glm::vec3(-1, -1, 0), glm::vec3(.2, .2, .2)),
		spotLight(glm::vec3(50.0f, 50.0f, 2.0f), glm::vec3(-1, -1, 0), glm::vec3(1.0)),
		bullethandler("resources/Bullet_Ours/LIGHTSABER.obj", "resources/Bullet_Ours/pure_blue.png", "resources/Bullet_Enemy/pure_red.png"),
		//ground("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::mat4{ 1.0f }, glm::vec3(0, 30, 0))),
		ground("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(0,20,0))),
		ground2("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(500, 20, 0))),
		ground3("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(-500, 20, 0))),
		ground4("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(0, 20, 500))),
		ground5("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(0, 20, -500))),
		ground6("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(-500, 20, 500))),
		ground7("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(-500, 20, -500))),
		ground8("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(500, 20, -500))),
		ground9("resources/moonsurface/moonsurface.obj", "resources/moonsurface/moon.jpg", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 1.0 }), glm::vec3(500, 20, 500))),
		red_glass1("resources/cube_transparent/cube_transparent.obj", "resources/cube_transparent/red.png", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 50.0 }), glm::vec3(5, 0, 0))), 
		red_glass2("resources/cube_transparent/cube_transparent.obj", "resources/cube_transparent/red.png", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 50.0 }), glm::vec3(-5, 0, 0))) ,
		red_glass3("resources/cube_transparent/cube_transparent.obj", "resources/cube_transparent/red.png", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 50.0 }), glm::vec3(0, 0, 5))),
		red_glass5("resources/cube_transparent/cube_transparent.obj", "resources/cube_transparent/red.png", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{ 50.0 }), glm::vec3(0, 0, 0))) ,
		red_glass4("resources/cube_transparent/cube_transparent.obj", "resources/cube_transparent/red.png", glm::translate(glm::scale(glm::mat4{ 1.0 }, glm::vec3{50.0}), glm::vec3(0, 0, -5))) {

		m_window.registerKeyCallback(
			[this](int key, int scancode, int action, int mods) {
				if (action == GLFW_PRESS)
					onKeyPressed(key, mods);
				else if (action == GLFW_RELEASE)
					onKeyReleased(key, mods);
			});
		m_window.registerMouseMoveCallback(
			std::bind(&Application::onMouseMove, this, std::placeholders::_1));
		m_window.registerMouseButtonCallback(
			[this](int button, int action, int mods) {
				if (action == GLFW_PRESS)
					onMouseClicked(button, mods);
				else if (action == GLFW_RELEASE)
					onMouseReleased(button, mods);
			});

		try {
			ShaderBuilder defaultBuilder;
			defaultBuilder.addStage(GL_VERTEX_SHADER,
									"shaders/phong_shader_vert.glsl");
			defaultBuilder.addStage(GL_FRAGMENT_SHADER,
									"shaders/phong_shader_frag.glsl");
			m_defaultShader = defaultBuilder.build();

			ShaderBuilder shadowBuilder;
			shadowBuilder.addStage(GL_VERTEX_SHADER,
								   "shaders/shadow_vert.glsl");
			m_shadowShader = shadowBuilder.build();

			ShaderBuilder toonBuilder;
			toonBuilder.addStage(GL_VERTEX_SHADER,
												 "shaders/phong_shader_vert.glsl");
			toonBuilder.addStage(GL_FRAGMENT_SHADER, "shaders/toon_shader_frag.glsl");
			m_toonShader = toonBuilder.build();
			// Any new shaders can be added below in similar fashion.
			// ==> Don't forget to reconfigure CMake when you do!
			//     Visual Studio: PROJECT => Generate Cache for ComputerGraphics
			//     VS Code: ctrl + shift + p => CMake: Configure => enter
			// ....
		} catch (ShaderLoadingException e) {
			std::cerr << e.what() << std::endl;
		}
	}

	void update() {

		GLuint texShadow;
		const int SHADOWTEX_WIDTH = 1024;
		const int SHADOWTEX_HEIGHT = 1024;
		glCreateTextures(GL_TEXTURE_2D, 1, &texShadow);
		glTextureStorage2D(texShadow, 1, GL_DEPTH_COMPONENT32F, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT);

		// Set behaviour for when texture coordinates are outside the [0, 1] range.
		glTextureParameteri(texShadow, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texShadow, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set interpolation for texture sampling (GL_NEAREST for no interpolation).
		glTextureParameteri(texShadow, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texShadow, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		// === Create framebuffer for extra texture ===
		GLuint framebuffer;
		glCreateFramebuffers(12, &framebuffer);
		glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, texShadow, 0);

		int dummyInteger = 0;  // Initialized to 0
		while (!m_window.shouldClose()) {
			// This is your game loop
			// Put your real-time logic and rendering in here
			m_window.updateInput();
			robot.update(player.getLocation(), bullethandler);

			// Use ImGui for easy input/output of ints, floats, strings, etc...
			ImGui::Begin("Window");
			//ImGui::InputInt(
				//"This is an integer input",
				//&dummyInteger);	 // Use ImGui::DragInt or ImGui::DragFloat for
			// larger range of numbers.
			ImGui::Text("Score is: %i",
				score);	// Use C printf formatting rules (%i is
			ImGui::Text("Damage taken is %i", damageTaken);
			// a signed integer)
			ImGui::Checkbox("Top View", &topviewEnabled);
			ImGui::End();
			// movement logic main character/mesh_1
//m_defaultShader.bind();






			// forward/backward
			if (goingForwards) { 
				player.setModelMatrix(glm::translate(player.getModelMatrix(), glm::vec3(0.0, 0.0, -1)));
			}
			else if (goingBackwards) {
				player.setModelMatrix(glm::translate(player.getModelMatrix(), glm::vec3(0.0, 0.0, 1)));
			}

			// left/right
			if (goingLeft) {
				player.setModelMatrix(glm::translate(player.getModelMatrix(), glm::vec3(-.7, 0.0, 0)));
			}
			else if (goingRight) {
				player.setModelMatrix(glm::translate(player.getModelMatrix(), glm::vec3(.7, 0.0, 0)));
			}

			//shooting
			if (shooting && shootCooldown <= 0) {
				bullethandler.createBullet(glm::translate(player.getModelMatrix(), glm::vec3(1, 0, 0)), true);
				bullethandler.createBullet(glm::translate(player.getModelMatrix(), glm::vec3(-1,0,0)), true);
				shootCooldown = 30;
			}
			shootCooldown--;
			std::cout << glm::dot(glm::normalize(spotLight.direction), glm::normalize(player.getLocation() - spotLight.position)) << "\n";
			//enemy1.shootAt(player.getLocation(), bullethandler);

			glm::mat4 shadowViewMatrix = glm::lookAt(spotLight.position, spotLight.position + spotLight.direction, glm::vec3(0, 1, 0));

			// toggle camera
			if (!topviewEnabled) {
				m_viewMatrix = glm::lookAt(glm::vec3(player.getModelMatrix() * glm::vec4(0, 3, 6, 1)), glm::vec3(player.getModelMatrix() * glm::vec4(0, 0, 0, 1)), glm::vec3(0, 1, 0));
			}
			else {
				m_viewMatrix = glm::lookAt(glm::vec3(player.getModelMatrix() * glm::vec4(0, 60, 6, 1)), glm::vec3(player.getModelMatrix() * glm::vec4(0, 0, 0, 1)), glm::vec3(0, 1, 0));
				//m_viewMatrix = glm::lookAt(m_lightPosition, glm::vec3(0.0), glm::vec3(0, 1, 0));
				//m_viewMatrix = shadowViewMatrix;
			}

			planets.update(0.001, 1.5f, 2.0f);


			{
				// Bind the off-screen framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

				// Clear the shadow map and set needed options
				glClearDepth(1.0f);
				glClear(GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				// Bind the shader
				m_shadowShader.bind();

				// Set viewport size
				glViewport(0, 0, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT);

				// .... HERE YOU MUST ADD THE CORRECT UNIFORMS FOR RENDERING THE SHADOW MAP

				//glDisable(GL_CULL_FACE);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				player.shadowDraw(m_projectionMatrix, shadowViewMatrix, framecounter);
				ground.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground2.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground3.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground4.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground5.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground6.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground7.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground8.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				ground9.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				bullethandler.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				enemy1.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				enemy2.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				enemy3.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				powerup1.shadowDraw(m_projectionMatrix, shadowViewMatrix);
				snake.shadowDraw(m_projectionMatrix, shadowViewMatrix, framecounter);
				robot.shadowDraw(m_projectionMatrix, shadowViewMatrix, framecounter);
				red_glass1.draw(m_projectionMatrix, shadowViewMatrix);
				// Execute draw command
				//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.triangles.size() * 3), GL_UNSIGNED_INT, nullptr);
				//glEnable(GL_CULL_FACE);
				// Unbind the off-screen framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				//glDisable(GL_BLEND);

			}


			if (player.isEmpowered()) {
				m_toonShader.bind();

			}
			else {
				m_defaultShader.bind();
			}

			// Clear the framebuffer to black and depth to maximum value
			glClearDepth(1.0f);
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glDisable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			// Bind the shadow map to texture slot 0
			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, texShadow);
			glUniform1i(13, 10);

			//Newly Added 3.27.2023
			// Set light properties
			glUniform3fv(5, 1, glm::value_ptr(spotLight.position));
			glUniform3fv(6, 1, glm::value_ptr(spotLight.color));

			// Set material properties
			glUniform3fv(8, 1, glm::value_ptr(m_materialAmbient));
			glUniform3fv(9, 1, glm::value_ptr(m_materialDiffuse));
			glUniform3fv(10, 1, glm::value_ptr(m_materialSpecular));
			glUniform1f(11, materialShininess);

			glUniform3fv(15, 1, glm::value_ptr(secondaryLight.position));
			glUniform3fv(16, 1, glm::value_ptr(secondaryLight.color));
			
			bool collected = powerup1.tryCollect(player.getLocation());

			if (collected) {
				player.empower();
			}
			player.draw(m_projectionMatrix, m_viewMatrix, framecounter);

			//m_defaultShader.bind();
			std::vector<glm::vec3> enemyLocations = { enemy1.getLocation(), enemy2.getLocation(), enemy3.getLocation(), robot.getLocation()};
			bullethandler.draw(player.getLocation(), m_projectionMatrix, m_viewMatrix, score, damageTaken, enemyLocations);
			ground.draw(m_projectionMatrix, m_viewMatrix);
			ground2.draw(m_projectionMatrix, m_viewMatrix);
			ground3.draw(m_projectionMatrix, m_viewMatrix);
			ground4.draw(m_projectionMatrix, m_viewMatrix);
			ground5.draw(m_projectionMatrix, m_viewMatrix);
			ground6.draw(m_projectionMatrix, m_viewMatrix);
			ground7.draw(m_projectionMatrix, m_viewMatrix);
			ground8.draw(m_projectionMatrix, m_viewMatrix);
			ground9.draw(m_projectionMatrix, m_viewMatrix);
			enemy1.draw(m_projectionMatrix, m_viewMatrix, player.getLocation(), bullethandler, spotLight);
			enemy2.draw(m_projectionMatrix, m_viewMatrix, player.getLocation(), bullethandler, spotLight);
			enemy3.draw(m_projectionMatrix, m_viewMatrix, player.getLocation(), bullethandler, spotLight);
			powerup1.draw(m_projectionMatrix, m_viewMatrix);
			snake.draw(m_projectionMatrix, m_viewMatrix, framecounter);
			planets.draw(m_projectionMatrix, m_viewMatrix);
			robot.draw(m_projectionMatrix, m_viewMatrix, framecounter);
			std::vector<Environment*> transparent = {};
			struct less_than_key {
				Player &player;
				inline bool operator() (Environment* trans1, Environment* trans2) {
					return glm::length(trans1->getLocation() - player.getLocation()) > glm::length(trans2->getLocation() - player.getLocation());
				}
			};
			transparent.push_back(&red_glass1);
			transparent.push_back(&red_glass2);
			transparent.push_back(&red_glass3);
			transparent.push_back(&red_glass4);
			transparent.push_back(&red_glass5);

			std::sort(transparent.begin(), transparent.end(), less_than_key(player));

			for (auto it = begin(transparent); it != end(transparent); ++it) {
				(*it)->draw(m_projectionMatrix, m_viewMatrix);
			}

			//red_glass1.draw(m_projectionMatrix, m_viewMatrix);
			//red_glass2.draw(m_projectionMatrix, m_viewMatrix);
			//red_glass3.draw(m_projectionMatrix, m_viewMatrix);
			//red_glass4.draw(m_projectionMatrix, m_viewMatrix);

			// Processes input and swaps the window buffer
			m_window.swapBuffers();
			framecounter++;
		}
	}

	// In here you can handle key presses
	// key - Integer that corresponds to numbers in
	// https://www.glfw.org/docs/latest/group__keys.html mods - Any modifier
	// keys pressed, like shift or control
	void onKeyPressed(int key, int mods) {
		std::cout << "Key pressed: " << key << std::endl;
		if (key == 87) { // forward
			goingForwards = true;
		}
		if (key == 83) {
			goingBackwards = true;
		}
		if (key == 65) {
			goingLeft = true;
		}
		if (key == 68) {
			goingRight = true;
		}
		if (key == 32) {
			shooting = true;
		}
	}

	// In here you can handle key releases
	// key - Integer that corresponds to numbers in
	// https://www.glfw.org/docs/latest/group__keys.html mods - Any modifier
	// keys pressed, like shift or control
	void onKeyReleased(int key, int mods) {
		std::cout << "Key released: " << key << std::endl;
		if (key == 87) {
			goingForwards = false;
		}
		if (key == 83) {
			goingBackwards = false;
		}
		if (key == 65) {
			goingLeft = false;
		}
		if (key == 68) {
			goingRight = false;
		}
		if (key == 32) {
			shooting = false;
		}
	}

	// If the mouse is moved this function will be called with the x, y
	// screen-coordinates of the mouse
	void onMouseMove(const glm::dvec2& cursorPos) {
		std::cout << "Mouse at position: " << cursorPos.x << " " << cursorPos.y
				  << std::endl;
		if (mouse0Held) {
			startMouseX = endMouseX;
			startMouseY = endMouseY;
			endMouseX = cursorPos.x;
			endMouseY = cursorPos.y;

			player.setModelMatrix(glm::rotate(player.getModelMatrix(), glm::radians(-(startMouseX - endMouseX) / 5.0f), glm::vec3(0, 1, 0)));
			//m_viewMatrixForward = glm::rotate(m_viewMatrixForward, glm::radians(-(startMouseX - endMouseX) / 5.0f), glm::vec3(0, 1, 0));
			//m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-(startMouseY - endMouseY) / 5.0f), glm::vec3(0, 0, 1));
		}
		else {
			endMouseX = cursorPos.x;
			endMouseY = cursorPos.y;
		}
	}

	// If one of the mouse buttons is pressed this function will be called
	// button - Integer that corresponds to numbers in
	// https://www.glfw.org/docs/latest/group__buttons.html mods - Any modifier
	// buttons pressed
	void onMouseClicked(int button, int mods) {
		std::cout << "Pressed mouse button: " << button << std::endl;
		if (button == 0) {
			mouse0Held = true;
		}
	}

	// If one of the mouse buttons is released this function will be called
	// button - Integer that corresponds to numbers in
	// https://www.glfw.org/docs/latest/group__buttons.html mods - Any modifier
	// buttons pressed
	void onMouseReleased(int button, int mods) {
		std::cout << "Released mouse button: " << button << std::endl;
		if (button == 0) {
			mouse0Held = false;
		}
	}

	int factorial(int i) {
		return i==0? 1 : i*factorial(i-1);
	}

  private:
	Window m_window;

	// Shader for default rendering and for depth rendering
	Shader m_defaultShader;
	Shader m_shadowShader;
	Shader m_toonShader;

	
	Powerup powerup1;
	Player player;
	Enemy enemy1;
	Enemy enemy2;
	Enemy enemy3;
	BulletHandler bullethandler;
	Environment ground;
	Environment ground2;
	Environment ground3;
	Environment ground4;
	Environment ground5;
	Environment ground6;
	Environment ground7;
	Environment ground8;
	Environment ground9;
	Light spotLight;
	Light secondaryLight;
	Snake snake;
	Planet planets;
	Robot robot;
	Environment red_glass1;
	Environment red_glass2;
	Environment red_glass3;
	Environment red_glass4;
	Environment red_glass5;


	int shootCooldown = 30;
	// Projection and view matrices for you to fill in and use
	glm::mat4 m_projectionMatrix =
		glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
	glm::mat4 m_viewMatrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0, 1, 0));

	int damageTaken = 0;
	int score = 0;

	// Light properties
	



	// Material properties
	glm::vec3 m_materialAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 m_materialDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 m_materialSpecular = glm::vec3(0.7f, 0.7f, 0.7f);
	float materialShininess = 0.1f;
	glm::vec4 texColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


};

int main() {
	Application app;
	app.update();

	return 0;
}
