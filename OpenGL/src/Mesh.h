#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Texture.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh{
private:
	VertexArray* _va;
	VertexBuffer* _vb;
	IndexBuffer* _ib;
	void SetupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();
	void Draw(Shader& shader, Renderer& renderer);
};

