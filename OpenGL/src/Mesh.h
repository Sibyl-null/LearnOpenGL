#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    Mesh(std::vector<Vertex> vertices, 
        std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader);
    unsigned int GetVAO() const { return _VAO; }
private:
    unsigned int _VAO, _VBO, _IBO;

    void SetupMesh();
};