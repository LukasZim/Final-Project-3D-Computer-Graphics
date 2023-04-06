#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>
#include "bullethandler.cpp"
#include "light.cpp"


class Enemy {
	public:
		Enemy(std::string filepath, std::string texturepath, glm::mat4 initialMatrix, int movementSteps, std::vector<float> x_coor, std::vector<float> y_coor)
		:
			m_mesh(filepath),
			m_texture(texturepath)
		{
			m_modelMatrix = initialMatrix;
			bezierSteps = movementSteps;
			steps = 0;

			
			// Bezier Curve
			int n = x_coor.size();
			for (float t = 0.0; t <= 1.0; t += (float) 1.0f/movementSteps) {
				
				
				//float x_coor[4] = { .08,.74,.81,.11 };
				//float y_coor[4] = { 1.3, 2.3, 3.3, 4.3 };
				float x_total = 0;
				float y_total = 0;
				for (int i = 0; i < n; i++) {
					auto var1 = factorial(n);
					auto var2 = (factorial(i) * factorial(n - i));
					auto var3 = std::pow(1 - t, n - i);
					auto var4 = std::pow(t, i);
					auto var5 = x_coor.at(i);
					x_total += (var1 / var2) * var3 * var4 * var5;
				}
				for (int i = 0; i < n; i++) {
					y_total += factorial(n) / (factorial(i) * factorial(n - i)) * std::pow(1 - t, n - i) * std::pow(t, i) * y_coor.at(i);
				}

				float final_x_coor = std::accumulate(x_coor.begin(), x_coor.end(), 0.0f);
				float final_y_coor = std::accumulate(y_coor.begin(), y_coor.end(), 0.0f);

				std::pair pair = std::make_pair(x_total, y_total);
				bezier_curve.push_back(pair);
			}
		}


		void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, glm::vec3 playerPos, BulletHandler& bullethandler, Light spotLight) {
			//m_modelMatrix = glm::translate(glm::rotate(m_modelMatrix, glm::radians((float)1.0f), glm::vec3(0, 1, 0)), glm::vec3(0, 0, 0));
			m_modelMatrix = glm::translate(glm::mat4{1.0}, glm::vec3(bezier_curve.at(steps ).first, 0.0f, bezier_curve.at(steps).second));

			const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
			const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));

			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
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
			glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(lightMVP));
			m_mesh.draw();
			//steps = (steps + 1) % (bezierSteps);
			if (increasing) {
				steps = steps + 1;
			}
			else {
				steps = steps - 1;
			}
			if (steps <= 0) {
				increasing = true;
			} 

			if (steps == bezierSteps - 1) {
				increasing = false;
			}


			shootAt(playerPos, bullethandler, spotLight);
			shootTimer--;
		}

		void shadowDraw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {
			lightMVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVP));
			m_mesh.draw();
		}

		glm::vec3 getLocation() {
			return m_modelMatrix * glm::vec4(0, 0, 0, 1);
		}

		void shootAt(glm::vec3 playerPos, BulletHandler& bullethandler, Light spotLight) {
			if (shootTimer <= 0 && glm::dot(glm::normalize(spotLight.direction), glm::normalize(playerPos - spotLight.position)) > 0.7) {
				bullethandler.createBullet(glm::inverse(glm::lookAt(getLocation(), playerPos, glm::vec3(0, 1, 0))), false);
				shootTimer = 30;
			}
			else if (shootTimer <= 0 && glm::length(playerPos - getLocation()) < 300) {
				bullethandler.createBullet(glm::inverse(glm::lookAt(getLocation(), playerPos, glm::vec3(0, 1, 0))), false);
				shootTimer = 30;
			}
		}
	private:
		GPUMesh m_mesh;
		glm::mat4 m_modelMatrix;
		glm::mat4 lightMVP;
		Texture m_texture;
		int steps;
		int bezierSteps;
		int shootTimer = 30;

		bool increasing = true;

		//Bezier Curve
		std::vector<std::pair<float, float>> bezier_curve; //x, y

		int factorial(int i) {
			return i == 0 ? 1 : i * factorial(i - 1);
		}
};