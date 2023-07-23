#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

Mesh::~Mesh()
{
	delete _va;
	delete _vb;
	delete _ib;
}

void Mesh::SetupMesh()
{
	_va = new VertexArray();
	_vb = new VertexBuffer(vertices.data(), sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	_va->AddBuffer(*_vb, layout);

	_ib = new IndexBuffer(indices.data(), indices.size());

	_va->UnBind();
	_vb->UnBind();
}

void Mesh::Draw(Shader& shader, Renderer& renderer)
{
	for (int i = 0; i < textures.size(); ++i) {
		Texture texture = textures[i];
		if (texture.type == TextureType::texture_diffuse) {
			texture.Bind(i);
			shader.SetUniform1i("material.diffuse", texture.GetSlot());
		}
		else if (texture.type == TextureType::texture_specular) {
			texture.Bind(i);
			shader.SetUniform1i("material.specular", texture.GetSlot());
		}
	}

	renderer.Draw(*_va, *_ib, shader);
}