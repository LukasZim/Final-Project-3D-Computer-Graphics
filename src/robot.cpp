#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "enemy.cpp"



class Robot {
public:
	Robot(glm::mat4 m) :
		m_mesh_1("resources/animation/animation1.obj"),
		m_mesh_2("resources/animation/animation2.obj"),
		m_mesh_3("resources/animation/animation3.obj"),
		m_mesh_4("resources/animation/animation4.obj"),
		m_mesh_5("resources/animation/animation5.obj"),
		m_mesh_6("resources/animation/animation6.obj"),
		m_mesh_7("resources/animation/animation7.obj"),
		m_mesh_8("resources/animation/animation8.obj"),
		m_mesh_9("resources/animation/animation9.obj"),
		m_mesh_10("resources/animation/animation10.obj"),
		m_mesh_11("resources/animation/animation11.obj"),
		m_mesh_12("resources/animation/animation12.obj"),
		texture("resources/animation/robot.jpg")
	{
		this->m_modelMatrix = m;
	}

	void shadowDraw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, int framecounter) {
		lightMVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVP));
		drawMesh(framecounter);
	}

	void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, int framecounter) {
		const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
		const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
		glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
		if (m_mesh_1.hasTextureCoords()) {
			texture.bind(GL_TEXTURE1);
			glUniform1i(3, 1);
			glUniform1i(4, GL_TRUE);
		}
		else {
			glUniform1i(4, GL_FALSE);
		}
		glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(lightMVP));
		drawMesh(framecounter);
	}

	glm::vec3 getLocation() {
		return m_modelMatrix * glm::vec4(0, 0, 0, 1);
	}

	void update(glm::vec3 playerPos, BulletHandler& bullethandler) {
		shootAt(playerPos, bullethandler);
		m_modelMatrix = glm::rotate(glm::inverse(glm::lookAt(getLocation(), playerPos, glm::vec3(0, 1, 0))), glm::radians(180.0f), glm::vec3(0,1,0));
		m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(20.0f));
		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0, 0, .01));
		shoottimer--;
	}

	void shootAt(glm::vec3 playerPos, BulletHandler& bullethandler) {
		if (shoottimer <= 0) {
			bullethandler.createBullet(glm::inverse(glm::lookAt(getLocation() + +glm::vec3(0, 0, 0), playerPos, glm::vec3(0, 1, 0))), false);
			bullethandler.createBullet(glm::inverse(glm::lookAt(getLocation() + glm::vec3(0, 60, 0), playerPos, glm::vec3(0, 1, 0))), false);
			bullethandler.createBullet(glm::inverse(glm::lookAt(getLocation() + glm::vec3(-15, 0, 0), playerPos, glm::vec3(0, 1, 0))), false);
			bullethandler.createBullet(glm::inverse(glm::lookAt(getLocation() + glm::vec3(15, 0, 0), playerPos, glm::vec3(0, 1, 0))), false);
			shoottimer = 15;
		}
	}

	void drawMesh(int framecounter) {
		float ans = framecounter % (12 * framesPerMesh) / framesPerMesh;
		if (ans < 1) {
			m_mesh_1.draw();
		} else if (ans < 2) {
			m_mesh_2.draw();
		}
		else if (ans < 3) {
			m_mesh_3.draw();
		}
		else if (ans < 4) {
			m_mesh_4.draw();
		}
		else if (ans < 5) {
			m_mesh_5.draw();
		}
		else if (ans < 6) {
			m_mesh_6.draw();
		}
		else if (ans < 7) {
			m_mesh_7.draw();
		}
		else if (ans < 8) {
			m_mesh_8.draw();
		}
		else if (ans < 9) {
			m_mesh_9.draw();
		}
		else if (ans < 10) {
			m_mesh_10.draw();
		}
		else if (ans < 11) {
			m_mesh_11.draw();
		}
		else if (ans < 12) {
			m_mesh_12.draw();
		}
	}

private:
	GPUMesh m_mesh_1;
	GPUMesh m_mesh_2;
	GPUMesh m_mesh_3;
	GPUMesh m_mesh_4;
	GPUMesh m_mesh_5;
	GPUMesh m_mesh_6;
	GPUMesh m_mesh_7;
	GPUMesh m_mesh_8;
	GPUMesh m_mesh_9;
	GPUMesh m_mesh_10;
	GPUMesh m_mesh_11;
	GPUMesh m_mesh_12;

	Texture texture;

	glm::mat4 m_modelMatrix;
	glm::mat4 lightMVP;

	int framesPerMesh = 5;
	int shoottimer = 15;
};