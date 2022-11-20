/**
 * @file mesh.hpp
 * @author Jovan Ivosevic
 * @brief Model mesh, a renderable piece of a model
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <assimp/scene.h>
#include<vector>
#include "ibufferable.hpp"

class Mesh : public IBufferable {
public:
    std::vector<unsigned> mIndices;
    std::vector<float> mVertices;

    /**
     * @brief Ctor - buffers mesh data
     *
     * @param mesh - Assimp mesh
     * @param MeshMaterial - Assimp material
     * @param resPath - Resource relative path. For loading textures, etc...
     * 
     */
    Mesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath);

	float* GetVertices();
	unsigned* GetIndices();
    unsigned GetVertexCount();
	unsigned GetIndexCount();
private:
    void processMesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath);
};