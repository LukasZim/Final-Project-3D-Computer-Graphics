#pragma once
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
DISABLE_WARNINGS_POP()
#include <filesystem>
#include <vector>

struct Image {
public:
    Image(const std::filesystem::path& filePath);

    glm::vec4 getTexel(const glm::vec2& textureCoordinates) const;

public:
    int width, height;
    std::vector<glm::vec4> pixels;
};
