#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Snake {
public:
	Snake(std::string meshpath_head, std::string meshpath_body, std::string meshpath_tail, std::string texturepath, glm::mat4 initialMatrix) :
		m_mesh_body(meshpath_body),
		m_mesh_head(meshpath_head),
		m_mesh_tail(meshpath_tail),
		m_texture(texturepath)
	{
		this->m_modelMatrix = initialMatrix;
		this->lightMVPBodies = {};
	}

	void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, int framecounter) {
		glm::mat4 updatedModelMatrix = m_modelMatrix;
		drawElement(m_mesh_head, m_projectionMatrix, m_viewMatrix, updatedModelMatrix, lightMVPHead);
		for (int i = 0; i < bodyParts; i++) {
			updatedModelMatrix = updateModelMatrix(updatedModelMatrix, framecounter, i);

			drawElement(m_mesh_body, m_projectionMatrix, m_viewMatrix, updatedModelMatrix, lightMVPBodies[i]);
		}
		updatedModelMatrix = updateModelMatrix(updatedModelMatrix, framecounter, bodyParts);

		drawElement(m_mesh_tail, m_projectionMatrix, m_viewMatrix, updatedModelMatrix, lightMVPTail);
	}

	void shadowDraw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, int framecounter) {
		lightMVPHead = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVPHead));
		m_mesh_head.draw();
		glm::mat4 updatedModelMatrix = m_modelMatrix;
		for (int i = 0; i < bodyParts; i++) {
			updatedModelMatrix = updateModelMatrix(updatedModelMatrix, framecounter, i);
			lightMVPBodies.push_back(m_projectionMatrix * m_viewMatrix * updatedModelMatrix);
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVPBodies.at(i)));
			m_mesh_body.draw();
		}
		updatedModelMatrix = updateModelMatrix(updatedModelMatrix, framecounter, bodyParts);

		lightMVPTail = m_projectionMatrix * m_viewMatrix * updatedModelMatrix;
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVPTail));
		m_mesh_body.draw();
	}

	glm::mat4 updateModelMatrix(glm::mat4 m, int framecounter, int offset) {
		float mult = 30.0f * sin((float)framecounter / 30 + offset);// *((offset + framecounter / 30) % 6 < 3 ? 1.0f : -1.0f);
		return glm::rotate(glm::translate(m, glm::vec3(0, 0, -bodyLenght)), glm::radians(mult), glm::vec3(0, 1, 0));

	}

	void drawElement(GPUMesh& m_mesh, glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, glm::mat4 updatedModelMatrix, glm::mat4 lightMVP) {
		//m_modelMatrix = glm::translate(glm::rotate(m_modelMatrix, glm::radians((float)1.0f), glm::vec3(0, 1, 0)), glm::vec3(0, 0, 0));
		const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * updatedModelMatrix;
		const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(updatedModelMatrix));
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
		glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
		if (m_mesh.hasTextureCoords()) {
			m_texture.bind(GL_TEXTURE1);
			glUniform1i(3, 1);
			glUniform1i(4, GL_TRUE);
		}
		else {
			glUniform1i(4, GL_FALSE);
		}
		glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(lightMVP));

		m_mesh.draw();
	}

private:
	GPUMesh m_mesh_head;
	GPUMesh m_mesh_body;
	GPUMesh m_mesh_tail;

	glm::mat4 m_modelMatrix;

	glm::mat4 lightMVPHead;
	std::vector<glm::mat4> lightMVPBodies;
	glm::mat4 lightMVPTail;


	int bodyParts = 10;
	float bodyLenght = 0.20;

	Texture m_texture;
};