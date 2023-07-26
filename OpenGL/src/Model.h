#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model
{
public:
    Model(std::string path);
    void Draw(Shader& shader);
private:
    std::vector<Mesh> _meshes;
    std::string _directory;
    std::vector<Texture> _textures_loaded;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
        TextureType typeEnum);
};