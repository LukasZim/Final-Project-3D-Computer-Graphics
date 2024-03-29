#pragma once
#include <exception>
#include <filesystem>
#include <framework/opengl_includes.h>
#include "texture.h"

// for version below C++17 uncomment below
// #include <experimental/optional>

struct MeshLoadingException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class GPUMesh {
public:
    GPUMesh(std::filesystem::path filePath);
    // Cannot copy a GPU mesh because it would require reference counting of GPU resources.
    GPUMesh(const GPUMesh&) = delete;
    GPUMesh(GPUMesh&&);
    ~GPUMesh();

    // Cannot copy a GPU mesh because it would require reference counting of GPU resources.
    GPUMesh& operator=(const GPUMesh&) = delete;
    GPUMesh& operator=(GPUMesh&&);

    bool hasTextureCoords() const;
    
    std::optional<Texture> kdTexture;
    // for version below C++17 use experimental::optional (and uncomment include above)
    // std::experimental::optional<Texture> kdTexture;

    // Bind VAO and call glDrawElements.
    void draw();

private:
    void moveInto(GPUMesh&&);
    void freeGpuMemory();

private:
    static constexpr GLuint INVALID = 0xFFFFFFFF;

    GLsizei m_numIndices { 0 };
    bool m_hasTextureCoords { false };
    GLuint m_ibo { INVALID };
    GLuint m_vbo { INVALID };
    GLuint m_vao { INVALID };
};
