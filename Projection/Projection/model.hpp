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
#include "buffer.hpp"


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
    std::vector<Buffer> mMeshBuffers;
    unsigned mVAO;
    unsigned mNumVertices;
    unsigned mNumIndices;

public:
    std::string mFilename;
    std::string mDirectory;
    glm::mat4 mModel;
    glm::vec3 mPosition;
    glm::vec3 mRotation;
    glm::vec3 mScale;

    Model(std::string filename);
    bool Load();
    void Render(Shader &program);

};

#define MESH_HP
#endif
