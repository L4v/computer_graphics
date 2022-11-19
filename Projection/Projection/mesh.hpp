#pragma once

#include <assimp/scene.h>
#include<vector>
#include "ibufferable.hpp"

class Mesh : public IBufferable {
public:
    std::vector<unsigned> mIndices;
    std::vector<float> mVertices;

    Mesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath);

	float* GetVertices();
	unsigned* GetIndices();
    unsigned GetVertexCount();
	unsigned GetIndexCount();
private:
    void processMesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath);
};