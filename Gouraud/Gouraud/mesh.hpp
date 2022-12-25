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
#include <GL/glew.h>;

class Mesh {
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
    Mesh(const aiMesh* mesh);

    /**
     * @brief Renders the current mesh
     *
     */
    void Render() const;

private:
    unsigned mVAO;
    unsigned mVBO;
    unsigned mEBO;
    unsigned mVertexCount;
    unsigned mIndexCount;
    void processMesh(const aiMesh* mesh);
};