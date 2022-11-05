#pragma once

#include <assimp/scene.h>
#include<vector>
#include "texture.hpp"
#include "irenderobject.hpp"

class Mesh : public IRenderObject {
public:
    std::vector<Texture>  mTextures;
    std::vector<unsigned> mIndices;
    std::vector<float> mVertices;

    Mesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath);

	float* GetVertices();
	unsigned* GetIndices();
    unsigned GetVertexCount();
	unsigned GetIndexCount();
private:
    void loadTextures(aiMaterial* material, Texture::Type type, const std::string& dir);
    void processMesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath);
};