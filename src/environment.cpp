#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Environment {
public:
	Environment(std::string meshpath, std::string texturepath, glm::mat4 startMatrix)
		:
		m_mesh(meshpath),
		m_texture(texturepath)
	{
		m_modelMatrix = startMatrix;
	}
	glm::vec3 getLocation() {
		return m_modelMatrix * glm::vec4(0, 0, 0, 1);
	}
	void setModelMatrix(glm::mat4 modelmatrix) {
		m_modelMatrix = modelmatrix;
	}
	glm::mat4 getModelMatrix() {
		return m_modelMatrix;
	}
	void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {
		//m_modelMatrixEnvironment = glm::translate(glm::rotate(m_modelMatrixEnvironment, glm::radians((float)1.0f), glm::vec3(0, 1, 0)), glm::vec3(0, 0, 0));
		const glm::mat4 mvpMatrixEnvironment = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
		const glm::mat3 normalModelMatrixEnvironment = glm::inverseTranspose(glm::mat3(m_modelMatrix));
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrixEnvironment));
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
		glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrixEnvironment));
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

	void shadowDraw(glm::mat4 m_projectionMatrix, glm::vec3 lightPos) {
		lightMVP = m_projectionMatrix * glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0, 1, 0)) * m_modelMatrix;
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVP));
		m_mesh.draw();
	}

private:
	GPUMesh m_mesh;
	Texture m_texture;
	glm::mat4 m_modelMatrix;
	glm::mat4 lightMVP;
};