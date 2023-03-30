#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();

    bool createDepthTexture();
    void bind();
    void unbind();

    GLuint getFramebufferID() const { return framebuffer; }
    GLuint getDepthMapTextureID() const { return depthMap; }
    glm::ivec2 getSize() const { return size; }

private:
    GLuint framebuffer = 0;
    GLuint depthMap = 0;
    glm::ivec2 size = { 1024,1024 };
};
