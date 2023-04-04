#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Snake {
public:
    Snake(std::string meshpath_head, std::string meshpath_body1, std::string meshpath_body2, std::string meshpath_tail, std::string texturepath, glm::mat4 startMatrix)
        :
        m_mesh_snakehead(meshpath_head),
        m_mesh_snakebody1(meshpath_body1),
        m_mesh_snakebody2(meshpath_body2),
        m_mesh_snaketail(meshpath_tail),
        m_texture_snake(texturepath)
    {
        m_modelMatrixsnake_head = glm::scale(startMatrix, glm::vec3(30,30,30));
        m_modelMatrixsnake_body1 = glm::translate(m_modelMatrixsnake_head, glm::vec3(0.0f,  0.0f, -1.0f * head_length));
        m_modelMatrixsnake_body2 = glm::translate(m_modelMatrixsnake_body1, glm::vec3(0.0f, 0.0f, -1.0f * body1_length));
        m_modelMatrixsnake_tail = glm::translate(m_modelMatrixsnake_body2, glm::vec3(0.0f,  0.0f, -1.0f * body2_length));
    }

    // ... (other existing methods) ...

    void draw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {
        drawPart(m_projectionMatrix, m_viewMatrix, m_modelMatrixsnake_head, m_mesh_snakehead);
        drawPart(m_projectionMatrix, m_viewMatrix, m_modelMatrixsnake_body1, m_mesh_snakebody1);
        drawPart(m_projectionMatrix, m_viewMatrix, m_modelMatrixsnake_body2, m_mesh_snakebody2);
        drawPart(m_projectionMatrix, m_viewMatrix, m_modelMatrixsnake_tail, m_mesh_snaketail);
    }

private:
    void drawPart(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix, glm::mat4 modelMatrix, GPUMesh& mesh) {
        const glm::mat4 mvpMatrixsnake = m_projectionMatrix * m_viewMatrix * modelMatrix;
        const glm::mat3 normalModelMatrixsnake = glm::inverseTranspose(glm::mat3(modelMatrix));
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrixsnake));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrixsnake));

        if (mesh.hasTextureCoords()) {
            m_texture_snake.bind(GL_TEXTURE9);
            glUniform1i(3, 9);
            glUniform1i(4, GL_TRUE);
        }
        else {
            glUniform1i(4, GL_FALSE);
        }
        mesh.draw();
    }

    GPUMesh m_mesh_snakehead;
    GPUMesh m_mesh_snakebody1;
    GPUMesh m_mesh_snakebody2;
    GPUMesh m_mesh_snaketail;
    Texture m_texture_snake;
    glm::mat4 m_modelMatrixsnake_head;
    glm::mat4 m_modelMatrixsnake_body1;
    glm::mat4 m_modelMatrixsnake_body2;
    glm::mat4 m_modelMatrixsnake_tail;
    int head_length = 1;
    int body1_length = 1;
    int body2_length = 1;
    int tail_length = 1;
};