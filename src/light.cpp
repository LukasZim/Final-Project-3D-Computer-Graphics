#include "mesh.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
	Light(glm::vec3 position, glm::vec3 direction, glm::vec3 color) {
		this->direction = direction;
		this->position = position;
		this->color = color;
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
	}
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
private:

};