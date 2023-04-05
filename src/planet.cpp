#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Planet {
public:
    Planet(std::string meshpath_planet1, std::string meshpath_planet2, std::string meshpath_planet3, std::string meshpath_planet4, std::string texture1path, std::string texture2path,
        std::string texture3path, std::string texture4path, glm::mat4 startMatrix)

        :
        m_mesh_planet1(meshpath_planet1),
        m_mesh_planet2(meshpath_planet2),
        m_mesh_planet3(meshpath_planet3),
        m_mesh_planet4(meshpath_planet4),
        m_texture_1(texture1path),
        m_texture_2(texture2path),
        m_texture_3(texture3path),
        m_texture_4(texture4path)
        
    {
        m_modelMatrix_planet1 = glm::translate(startMatrix, glm::vec3(-300, 500, 500));
        m_modelMatrix_planet2 = glm::translate(m_modelMatrix_planet1, glm::vec3(radius, 0.0f, 0.0f));
        m_modelMatrix_planet3 = glm::translate(m_modelMatrix_planet2, glm::vec3(0.0f, 0.0f , -radius));
        m_modelMatrix_planet4 = glm::translate(m_modelMatrix_planet3, glm::vec3(-radius,  0.0f, 0.0f));
    }

    // ... (other existing methods) ...

    void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {
        drawPlanet(m_projectionMatrix, m_viewMatrix, m_modelMatrix_planet1, m_mesh_planet1, m_texture_1);
        drawPlanet(m_projectionMatrix, m_viewMatrix, m_modelMatrix_planet2, m_mesh_planet2, m_texture_2);
        drawPlanet(m_projectionMatrix, m_viewMatrix, m_modelMatrix_planet3, m_mesh_planet3, m_texture_3);
        drawPlanet(m_projectionMatrix, m_viewMatrix, m_modelMatrix_planet4, m_mesh_planet4, m_texture_4);
    }
    void update(float Time, float centralRotationSpeed, float selfRotationSpeed) {
        glm::mat4 centralRotationMatrix = glm::rotate(glm::mat4(1.0f), centralRotationSpeed * Time, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 selfRotationMatrix = glm::rotate(glm::mat4(1.0f), selfRotationSpeed * Time, glm::vec3(0.0f, 0.0f, 1.0f));

        m_modelMatrix_planet1 = centralRotationMatrix * m_modelMatrix_planet1 * selfRotationMatrix;
        m_modelMatrix_planet2 = centralRotationMatrix * m_modelMatrix_planet2 * selfRotationMatrix;
        m_modelMatrix_planet3 = centralRotationMatrix * m_modelMatrix_planet3 * selfRotationMatrix;
        m_modelMatrix_planet4 = centralRotationMatrix * m_modelMatrix_planet4 * selfRotationMatrix;
    }
    void drawPlanet(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, glm::mat4 modelMatrix, GPUMesh& mesh, Texture& texture) {
        const glm::mat4 mvpMatrixsnake = m_projectionMatrix * m_viewMatrix * modelMatrix;
        const glm::mat3 normalModelMatrixsnake = glm::inverseTranspose(glm::mat3(modelMatrix));
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrixsnake));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrixsnake));

        if (mesh.hasTextureCoords()) {
            texture.bind(GL_TEXTURE4);
            glUniform1i(3, 4);
            glUniform1i(4, GL_TRUE);
        }
        else {
            glUniform1i(4, GL_FALSE);
        }
        mesh.draw();
    }

private:
    GPUMesh m_mesh_planet1;
    GPUMesh m_mesh_planet2;
    GPUMesh m_mesh_planet3;
    GPUMesh m_mesh_planet4;
    Texture m_texture_1;
    Texture m_texture_2;
    Texture m_texture_3;
    Texture m_texture_4;
    glm::mat4 m_modelMatrix_planet1;
    glm::mat4 m_modelMatrix_planet2;
    glm::mat4 m_modelMatrix_planet3;
    glm::mat4 m_modelMatrix_planet4;
    float radius = 800.0f;
};