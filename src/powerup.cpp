#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Powerup {
	public:

		Powerup(std::string meshpath, std::string texturepath, glm::mat4 startMatrix)
			:
			m_mesh_powerup(meshpath),
			m_texture_powerup(texturepath)
		{
			collected = false;
			countDown = 0;
			m_modelMatrixPowerup = startMatrix;
		}

		glm::vec3 getLocation() {
			return m_modelMatrixPowerup * glm::vec4(0, 0, 0, 1);
		}

		void setModelMatrix(glm::mat4 modelmatrix){
			m_modelMatrixPowerup = modelmatrix;
		}

		glm::mat4 getModelMatrix() {
			return m_modelMatrixPowerup;
		}

		void collect() {
			this->collected = true;
			this->countDown = 200;
		}

		bool tryCollect(glm::vec3 location) {
			if (!collected && glm::length(getLocation() - location) < 18) {
				collect();
				return true;
			}
			return false;
		}
		/*
		bool tryCollect(Player &player) {
			if (glm::distance(getLocation(), player.getLocation()) < 10) {
				collect();
				return true;
			}
			return false;
		}*/

		void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {
			m_modelMatrixPowerup = glm::translate(glm::rotate(m_modelMatrixPowerup, glm::radians((float)1.0f), glm::vec3(0, 1, 0)), glm::vec3(0, 0, 0));

			const glm::mat4 mvpMatrixPowerup = m_projectionMatrix * m_viewMatrix * m_modelMatrixPowerup;
			const glm::mat3 normalModelMatrixPowerup = glm::inverseTranspose(glm::mat3(m_modelMatrixPowerup));

			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrixPowerup));
			glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrixPowerup));
			glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrixPowerup));
			if (m_mesh_powerup.hasTextureCoords()) {
				m_texture_powerup.bind(GL_TEXTURE1);
				//m_mesh2.kdTexture.value().bind(GL_TEXTURE1);
				glUniform1i(3, 1);
				glUniform1i(4, GL_TRUE);
			}
			else {
				glUniform1i(4, GL_FALSE);
			}
			if (collected) {
				countDown = countDown - 1;
				if (countDown < 0) {
					collected = false;
				}
			}
			else {
				m_mesh_powerup.draw();
			}
		}

		void shadowDraw(glm::mat4 m_projectionMatrix, glm::vec3 lightPos) {
			const glm::mat4 mvp = m_projectionMatrix * glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0, 1, 0)) * m_modelMatrixPowerup;
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));
			m_mesh_powerup.draw();
		}

	private:
		GPUMesh m_mesh_powerup;
		Texture m_texture_powerup;
		glm::mat4 m_modelMatrixPowerup;

		bool collected;
		int countDown;

};