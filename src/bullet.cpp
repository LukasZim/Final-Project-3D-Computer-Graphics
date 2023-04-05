#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <iostream>
#include <glm/gtx/string_cast.hpp>




class Bullet {
	public:
		bool friendly;
		Bullet(glm::mat4 startMatrix, int speed, int damage, int range, bool friendly):
			speed(speed),
			damage(damage),
			range(range),
			m_modelMatrix(startMatrix)
		{
			this->friendly = friendly;
		}

		glm::vec3 getLocation() {
			return m_modelMatrix * glm::vec4(0, 0, 0, 1);
		}

		bool outOfRange(glm::vec3 location) {
			if (glm::length(getLocation() - location) > 2000) {
				return true;
			}
			return false;
		}

		void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, GPUMesh *m_mesh, Texture *m_texture) {
			m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 0.0f, -0.3f));
			//std::cout << glm::to_string(getLocation()) << "\n";
			std::cout << speed << "\n";
			const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
			const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));

			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
			glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
			glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
			if ((*m_mesh).hasTextureCoords()) {
				(*m_texture).bind(GL_TEXTURE1);
				//m_mesh2.kdTexture.value().bind(GL_TEXTURE1);
				glUniform1i(3, 1);
				glUniform1i(4, GL_TRUE);
			}
			else {
				glUniform1i(4, GL_FALSE);
			}
			glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(lightMVP));
			(*m_mesh).draw();
		}

		void shadowDraw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, GPUMesh* m_mesh) {
			lightMVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVP));
			(*m_mesh).draw();
		}


	private:
		glm::mat4 m_modelMatrix;
		glm::mat4 lightMVP;

		int speed;
		int damage;
		int range;

};