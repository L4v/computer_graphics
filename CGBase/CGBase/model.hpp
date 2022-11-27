/**
 * @file model.hpp
 * @author Jovan Ivosevic
 * @brief Model wrapper class
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "mesh.hpp"


#define POSITION_LOCATION 0

#define POSTPROCESS_FLAGS (aiProcess_Triangulate | aiProcess_FlipUVs)
#define INVALID_MATERIAL 0xFFFFFFFF

enum EBufferType {
    INDEX_BUFFER = 0,
    POS_VB = 1,
    TEXCOORD_VB = 2,
    NORM_VB = 3,
    BUFFER_COUNT = 4,
};

class Model {
private:
    std::vector<Mesh> mMeshes;

public:
    std::string mFilename;
    std::string mDirectory;

    /**
     * @brief Ctor - sets up data for model loading in Assimp
     *
     * @param filename - Model path
     *
     */
    Model(std::string filename);

    /**
     * @brief Loads all the meshes and model data
     *
     * @returns true - Success, false - Failure
     */
    bool Load();

    /**
     * @brief Renderable Render implementation
     *
     */
    void Render();

};

#define MESH_HP
#endif
