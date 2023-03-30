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

class Application {
  public:
	  int startMouseX = 0;
	  int startMouseY = 0;

	  bool mouse0Held = false;
	  bool goingForwards = false;
	  bool goingBackwards = false;

	  int endMouseX = 0;
	  int endMouseY = 0;


	Application()
		: 
		m_window("Final Project", glm::ivec2(1024, 1024), OpenGLVersion::GL45),
		m_mesh2("resources/cube-textured.obj"),
		m_mesh("resources/dragon.obj"),
		m_mesh_ground("resources/test/test.obj"),
		m_texture("resources/checkerboard.png") ,
		m_texture_ground_1("resources/test/coral_fort_wall_01_diff_1k.jpg") ,
		m_texture_ground_2("resources/test/forest_ground_04_diff_1k.jpg") {
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
									"shaders/new_shader_vert.glsl");
			defaultBuilder.addStage(GL_FRAGMENT_SHADER,
									"shaders/new_shader_frag.glsl");
			m_defaultShader = defaultBuilder.build();

			ShaderBuilder shadowBuilder;
			shadowBuilder.addStage(GL_VERTEX_SHADER,
								   "shaders/shadow_vert.glsl");
			m_shadowShader = shadowBuilder.build();

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
		
		int dummyInteger = 0;  // Initialized to 0
		while (!m_window.shouldClose()) {
			// This is your game loop
			// Put your real-time logic and rendering in here
			m_window.updateInput();

			// Use ImGui for easy input/output of ints, floats, strings, etc...
			ImGui::Begin("Window");
			ImGui::InputInt(
				"This is an integer input",
				&dummyInteger);	 // Use ImGui::DragInt or ImGui::DragFloat for
			// larger range of numbers.
			ImGui::Text("Value is: %i",
				dummyInteger);	// Use C printf formatting rules (%i is
			// a signed integer)
			ImGui::End();


			// Clear the screen
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			//Newly Added 3.27.2023
			// Set light properties
			glUniform3fv(5, 1, glm::value_ptr(m_lightPosition));
			glUniform3fv(6, 1, glm::value_ptr(m_lightColor));

			// Set material properties
			glUniform3fv(8, 1, glm::value_ptr(m_materialAmbient));
			glUniform3fv(9, 1, glm::value_ptr(m_materialDiffuse));
			glUniform3fv(10, 1, glm::value_ptr(m_materialSpecular));
			glUniform1f(11, materialShininess);
			glUniform4fv(12, 1, glm::value_ptr(texColor));


			// movement logic main character/mesh_1
			m_defaultShader.bind();
			if (goingForwards) {
				m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.01, 0.0, 0.0));
			}
			else if (goingBackwards) {
				m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(-0.01, 0.0, 0.0));
			}
			m_viewMatrix = glm::lookAt(glm::vec3(m_modelMatrix * glm::vec4(-2, 1, 0, 1)), glm::vec3(m_modelMatrix * glm::vec4(0, 0, 0, 1)), glm::vec3(0, 1, 0));


			// ****** start mesh_1 logic ****** 
			const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

			// https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
			const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));

			if (m_mesh.hasTextureCoords()) {
				m_texture.bind(GL_TEXTURE0);
				//m_mesh.kdTexture.value().bind(GL_TEXTURE0);
				glUniform1i(3, 0);
				glUniform1i(4, GL_TRUE);
			} else {
				glUniform1i(4, GL_FALSE);
			}

			// Set view position
			glm::vec3 viewPosition = glm::vec3(m_viewMatrix[3]);
			glUniform3fv(7, 1, glm::value_ptr(viewPosition));
			
			// give matrices to the shader
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
			glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
			glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

			m_mesh.draw();
			// ****** end mesh_1 logic ****** 

			// ****** start mesh_2 logic ****** 
			m_modelMatrix2 = glm::rotate(m_modelMatrix2, glm::radians((float)dummyInteger), glm::vec3(0, 1, 0));

			const glm::mat4 mvpMatrix2 = m_projectionMatrix * m_viewMatrix * m_modelMatrix2;
			const glm::mat3 normalModelMatrix2 = glm::inverseTranspose(glm::mat3(m_modelMatrix2));

			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix2));
			glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrix2));
			glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix2));
			if (m_mesh2.hasTextureCoords()) {
				m_texture_ground_1.bind(GL_TEXTURE1);
				//m_mesh2.kdTexture.value().bind(GL_TEXTURE1);
				glUniform1i(3, 1);
				glUniform1i(4, GL_TRUE);
			}
			else {
				glUniform1i(4, GL_FALSE);
			}

			m_mesh2.draw();
			// ****** end mesh_2 logic ****** 

			//m_mesh_ground.draw();

			// Processes input and swaps the window buffer
			m_window.swapBuffers();
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

			m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-(startMouseX - endMouseX) / 5.0f), glm::vec3(0, 1, 0));
			//m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(-(startMouseX - endMouseX) / 5.0f), glm::vec3(0, 1, 0));
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


  private:
	Window m_window;

	// Shader for default rendering and for depth rendering
	Shader m_defaultShader;
	Shader m_shadowShader;

	GPUMesh m_mesh;
	GPUMesh m_mesh2;
	GPUMesh m_mesh_ground;

	Texture m_texture;
	Texture m_texture_ground_1;
	Texture m_texture_ground_2;

	// Projection and view matrices for you to fill in and use
	glm::mat4 m_projectionMatrix =
		glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 30.0f);
	glm::mat4 m_viewMatrix =
		glm::lookAt(glm::vec3(0, 0, -2), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 m_modelMatrix{ 1.0f };
	glm::mat4 m_modelMatrix2{1.0f};


	// Light properties
	

	glm::vec3 m_lightPosition = glm::vec3(20.0f, 2.0f, 2.0f);
	glm::vec3 m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	// Material properties
	glm::vec3 m_materialAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 m_materialDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 m_materialSpecular = glm::vec3(0.7f, 0.7f, 0.7f);
	float materialShininess = 0.1f;
	glm::vec4 texColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float ambientIntensity = 0.1f;
};

int main() {
	Application app;
	app.update();

	return 0;
}
