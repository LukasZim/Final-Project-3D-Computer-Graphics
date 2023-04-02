#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Bullet {
	public:
		Bullet(std::string meshpath, std::string texturepath, glm::mat4 startMatrix) :
			m_mesh(meshpath),
			m_texture(texturepath)
		{
			m_modelMatrix = startMatrix;
		}

		void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {
			m_modelMatrix = glm::translate(glm::rotate(m_modelMatrix, glm::radians((float)1.0f), glm::vec3(0, 1, 0)), glm::vec3(0, 0, 0));

			const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
			const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));

			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
			glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
			glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
			if (m_mesh.hasTextureCoords()) {
				m_texture.bind(GL_TEXTURE1);
				//m_mesh2.kdTexture.value().bind(GL_TEXTURE1);
				glUniform1i(3, 1);
				glUniform1i(4, GL_TRUE);
			}
			else {
				glUniform1i(4, GL_FALSE);
			}
		}
	private:
		GPUMesh m_mesh;
		Texture m_texture;
		glm::mat4 m_modelMatrix;

};