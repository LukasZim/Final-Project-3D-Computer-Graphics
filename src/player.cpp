#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Player {
	public:
		Player(std::string texturepath, glm::mat4 startMatrix)
			: 
			m_mesh("resources/Gunship_Model/gunship1.obj"),
			m_texture(texturepath)/*,
			
			m_mesh1("resources/Gunship_Model/gunship1.obj"),
			m_mesh2("resources/Gunship_Model/gunship2.obj"),
			m_mesh3("resources/Gunship_Model/gunship3.obj"),
			m_mesh4("resources/Gunship_Model/gunship4.obj"),
			m_mesh5("resources/Gunship_Model/gunship5.obj"),
			m_mesh6("resources/Gunship_Model/gunship6.obj"),
			m_mesh7("resources/Gunship_Model/gunship7.obj"),
			m_mesh8("resources/Gunship_Model/gunship8.obj"),
			m_mesh9("resources/Gunship_Model/gunship9.obj"),
			m_mesh10("resources/Gunship_Model/gunship10.obj"),
			m_mesh11("resources/Gunship_Model/gunship11.obj"),
			m_mesh12("resources/Gunship_Model/gunship12.obj"),
			m_mesh13("resources/Gunship_Model/gunship13.obj"),
			m_mesh14("resources/Gunship_Model/gunship14.obj"),
			m_mesh15("resources/Gunship_Model/gunship15.obj"),
			m_mesh16("resources/Gunship_Model/gunship16.obj"),
			m_mesh17("resources/Gunship_Model/gunship17.obj"),
			m_mesh18("resources/Gunship_Model/gunship18.obj"),
			m_mesh19("resources/Gunship_Model/gunship19.obj"),
			m_mesh20("resources/Gunship_Model/gunship20.obj")
			*/
		{
			//m_mesh = GPUMesh("resources/cube.obj");
			m_modelMatrix = startMatrix;
			empowered = false;
			duration = 0;
		}

		glm::mat4 getModelMatrix() {
			return this->m_modelMatrix;
		}

		void setModelMatrix(glm::mat4 m) {
			this->m_modelMatrix = m;
		}

		glm::vec3 getLocation() {
			return m_modelMatrix * glm::vec4(0, 0, 0, 1);
		}

		void empower() {
			if (!empowered) {
				m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{ 4.0 });
			}
			empowered = true;
			duration = 500;
		}

		bool isEmpowered() {
			return empowered;
		}

		void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, int framecounter) {
			// ****** start playermesh logic ****** 
			const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

			// https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
			const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));

			if (m_mesh.hasTextureCoords()) {
				m_texture.bind(GL_TEXTURE0);
				//m_mesh.kdTexture.value().bind(GL_TEXTURE0);
				glUniform1i(3, 0);
				glUniform1i(4, GL_TRUE);
			}
			else {
				glUniform1i(4, GL_FALSE);
			}

			// Set view position
			//glm::vec3 viewPosition = glm::vec3(m_viewMatrix[3]);
			//glUniform3fv(7, 1, glm::value_ptr(viewPosition));


			if (empowered) {
				glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
				glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
				glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

				if (duration <= 0) {
					empowered = false;
					m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{ 0.25 });
				}
				duration--;
			}
			else {

				// give matrices to the shader
				glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
				glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

			}
			glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(lightMVP));
			drawCorrectModel(framecounter);
			
		}

		void drawCorrectModel(int framecounter) {
			m_mesh.draw();
			// awful animation implementation :(
			/*
			switch ((framecounter % 80) / 2) {
			case 0:
			case 1:
				m_mesh1.draw();
				break;
			case 2:
			case 3:
				m_mesh2.draw();
				break;
			case 4:
			case 5:
				m_mesh3.draw();
				break;
			case 6:
			case 7:
				m_mesh4.draw();
				break;
			case 8:
			case 9:
				m_mesh5.draw();
				break;
			case 10:
			case 11:
				m_mesh6.draw();
				break;
			case 12:
			case 13:
				m_mesh7.draw();
				break;
			case 14:
			case 15:
				m_mesh8.draw();
				break;
			case 16:
			case 17:
				m_mesh9.draw();
				break;
			case 18:
			case 19:
				m_mesh10.draw();
				break;
			case 20:
			case 21:
				m_mesh11.draw();
				break;
			case 22:
			case 23:
				m_mesh12.draw();
				break;
			case 24:
			case 25:
				m_mesh13.draw();
				break;
			case 26:
			case 27:
				m_mesh14.draw();
				break;
			case 28:
			case 29:
				m_mesh15.draw();
				break;
			case 30:
			case 31:
				m_mesh16.draw();
				break;
			case 32:
			case 33:
				m_mesh17.draw();
				break;
			case 34:
			case 35:
				m_mesh18.draw();
				break;
			case 36:
			case 37:
				m_mesh19.draw();
				break;
			case 38:
			case 39:
				m_mesh20.draw();
				break;

			}
			*/
		}

		void shadowDraw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, int framecounter) {
			lightMVP = m_projectionMatrix *m_viewMatrix * m_modelMatrix;
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(lightMVP));
			drawCorrectModel(framecounter);
		}

	private:
		Texture m_texture;
		glm::mat4 m_modelMatrix;
		glm::mat4 lightMVP;

		bool empowered;
		int duration;


		GPUMesh m_mesh;
		/*
		GPUMesh m_mesh1;
		GPUMesh m_mesh2;
		GPUMesh m_mesh3;
		GPUMesh m_mesh4;
		GPUMesh m_mesh5;
		GPUMesh m_mesh6;
		GPUMesh m_mesh7;
		GPUMesh m_mesh8;
		GPUMesh m_mesh9;
		GPUMesh m_mesh10;
		GPUMesh m_mesh11;
		GPUMesh m_mesh12;
		GPUMesh m_mesh13;
		GPUMesh m_mesh14;
		GPUMesh m_mesh15;
		GPUMesh m_mesh16;
		GPUMesh m_mesh17;
		GPUMesh m_mesh18;
		GPUMesh m_mesh19;
		GPUMesh m_mesh20;
		*/

};