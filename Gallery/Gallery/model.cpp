#include "model.hpp"

#include "include/stb_image.h"

MeshInfo::MeshInfo() {
    mNumIndices    = 0;
    mBaseVertex    = 0;
    mBaseIndex     = 0;
    mMaterialIndex = INVALID_MATERIAL;
}

void
MeshInfo::LoadTextures(aiMaterial *material, Texture::Type type, const std::string &dir) {
    // TODO(Jovan): Map better
    aiTextureType aiType = type == Texture::DIFFUSE ? aiTextureType_DIFFUSE : aiTextureType_SPECULAR;
    for(unsigned TexIdx = 0; TexIdx < material->GetTextureCount(aiType); ++TexIdx) {
        aiString TexName;
        material->GetTexture(aiType, TexIdx, &TexName);
        // TODO(Jovan): Check for duplicates
        Texture Tex(dir + '/' + TexName.C_Str(), type);
        mTextures.push_back(Tex);
    }
}

Model::Model(std::string filename) {
    mFilename = filename;
    mDirectory = filename.substr(0, filename.find_last_of('/'));
    mNumVertices = 0;
    mNumIndices = 0;
    mBuffers.resize(BUFFER_COUNT);
}

bool
Model::Load() {
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glGenBuffers(BUFFER_COUNT, &mBuffers[0]);

    bool Result = false;
    Assimp::Importer Importer;

    const aiScene *Scene = Importer.ReadFile(mFilename, POSTPROCESS_FLAGS);

    if(!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
        std::cerr << "[Err] Failed to load model:" << std::endl << Importer.GetErrorString() << std::endl;
        Result = false;
    } else {
        mMeshes.resize(Scene->mNumMeshes);
        mTextures.resize(Scene->mNumMaterials);
        for(unsigned MeshIndex = 0; MeshIndex < mMeshes.size(); ++MeshIndex) {
            mMeshes[MeshIndex].mMaterialIndex = Scene->mMeshes[MeshIndex]->mMaterialIndex;
            mMeshes[MeshIndex].mNumIndices = Scene->mMeshes[MeshIndex]->mNumFaces * 3;
            mMeshes[MeshIndex].mBaseVertex = mNumVertices;
            mMeshes[MeshIndex].mBaseIndex = mNumIndices;

            mNumVertices += Scene->mMeshes[MeshIndex]->mNumVertices;
            mNumIndices += mMeshes[MeshIndex].mNumIndices;
        }

        mPositions.reserve(mNumVertices);
        mNormals.reserve(mNumVertices);
        mTexCoords.reserve(mNumVertices);
        mIndices.reserve(mNumIndices);

        for(unsigned MeshIdx = 0; MeshIdx < mMeshes.size(); ++MeshIdx) {
            Model::ProcessMesh(Scene, Scene->mMeshes[MeshIdx], MeshIdx);
        }
        // NOTE(Jovan): Populate buffers
        glBindBuffer(GL_ARRAY_BUFFER, mBuffers[POS_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mPositions) * mPositions.size(), &mPositions[0], GL_STATIC_DRAW);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(POSITION_LOCATION);

        glBindBuffer(GL_ARRAY_BUFFER, mBuffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoords) * mTexCoords.size(), &mTexCoords[0], GL_STATIC_DRAW);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(TEX_COORD_LOCATION);

        glBindBuffer(GL_ARRAY_BUFFER, mBuffers[NORM_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mNormals) * mNormals.size(), &mNormals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(NORMAL_LOCATION);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices[0]) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        std::cout << "Loaded " << mMeshes.size() << " meshes" << std::endl;

        Result = true;
    }
    
    glBindVertexArray(0);
    return Result;
}

void
Model::ProcessMesh(const aiScene *scene, const aiMesh *mesh, unsigned meshIdx) {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for(unsigned VertexIndex = 0; VertexIndex < mesh->mNumVertices; ++VertexIndex) {
        const aiVector3D &TexCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][VertexIndex] : Zero3D;

        mPositions.push_back(glm::vec3(mesh->mVertices[VertexIndex].x, mesh->mVertices[VertexIndex].y, mesh->mVertices[VertexIndex].z));
        mNormals.push_back(glm::vec3(mesh->mNormals[VertexIndex].x, mesh->mNormals[VertexIndex].y, mesh->mNormals[VertexIndex].z));
        mTexCoords.push_back(glm::vec2(TexCoord.x, TexCoord.y));
    }

    for(unsigned FaceIndex = 0; FaceIndex < mesh->mNumFaces; ++FaceIndex) {
        const aiFace& Face = mesh->mFaces[FaceIndex];
        mIndices.push_back(Face.mIndices[0]);
        mIndices.push_back(Face.mIndices[1]);
        mIndices.push_back(Face.mIndices[2]);
    }

    // NOTE(Jovan): Load material data
    aiMaterial *MeshMaterial = scene->mMaterials[mesh->mMaterialIndex];

    if(mesh->mMaterialIndex >= 0) {
        std::cout << MeshMaterial->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
        std::cout << MeshMaterial->GetTextureCount(aiTextureType_SPECULAR) << std::endl;
        mMeshes[meshIdx].LoadTextures(MeshMaterial, Texture::DIFFUSE, mDirectory);
        mMeshes[meshIdx].LoadTextures(MeshMaterial, Texture::SPECULAR, mDirectory);
    }
}

void
Model::Render(const PhongShader &program) {
    mModel = glm::mat4(1.0f);
    mModel = glm::translate(mModel, mPosition);
    mModel = glm::rotate(mModel, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mModel = glm::rotate(mModel, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mModel = glm::rotate(mModel, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    mModel = glm::scale(mModel, mScale);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[INDEX_BUFFER]);
    for(unsigned MeshIdx = 0; MeshIdx < mMeshes.size(); ++MeshIdx) {
        MeshInfo &Mesh = mMeshes[MeshIdx];

        for(unsigned TexIdx = 0; TexIdx < Mesh.mTextures.size(); ++TexIdx) {
            Texture &Tex = Mesh.mTextures[TexIdx];
            // TODO(Jovan): Multiple same-type texture support
            program.BindTexture(Tex, TexIdx);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES,
                mMeshes[MeshIdx].mNumIndices,
                GL_UNSIGNED_INT,
                (void*)(sizeof(unsigned) * mMeshes[MeshIdx].mBaseIndex),
                mMeshes[MeshIdx].mBaseVertex);

        glActiveTexture(GL_TEXTURE0);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
