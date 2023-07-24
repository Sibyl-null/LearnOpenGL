#include "Mesh.h"
#include <string>

Mesh::Mesh(std::vector<Vertex> vertices, 
    std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        textures[i].Bind(i);

        if (textures[i].type == TextureType::texture_diffuse) {
            std::string number = std::to_string(diffuseNr++);
            shader.SetUniform1i("texture_diffuse" + number, i);
        }
        else if (textures[i].type == TextureType::texture_specular) {
            std::string number = std::to_string(specularNr++);
            shader.SetUniform1i("texture_specular" + number, i);
        }
    }
    GLCall(glActiveTexture(GL_TEXTURE0));

    GLCall(glBindVertexArray(_VAO));
    GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));
}

void Mesh::SetupMesh()
{
    GLCall(glGenVertexArrays(1, &_VAO));
    GLCall(glGenBuffers(1, &_VBO));
    GLCall(glGenBuffers(1, &_IBO));

    GLCall(glBindVertexArray(_VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, _VBO));

    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
        &vertices[0], GL_STATIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW));

    // 顶点位置
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
    // 顶点法线
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
    // 顶点 uv 坐标
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

    GLCall(glBindVertexArray(0));
}