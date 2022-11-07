#include "model.hpp"

#include "include/stb_image.h"

Model::Model(std::string filename) {
    mFilename = filename;
    mDirectory = filename.substr(0, filename.find_last_of('/'));
    mNumVertices = 0;
    mNumIndices = 0;
}

bool
Model::Load() {
    Assimp::Importer Importer;
    const aiScene *Scene = Importer.ReadFile(mFilename, POSTPROCESS_FLAGS);

    if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
        std::cerr << "[Err] Failed to load model:" << std::endl << Importer.GetErrorString() << std::endl;
        return false;
    }
    mMeshes.reserve(Scene->mNumMeshes);
    for(unsigned MeshIdx = 0; MeshIdx < Scene->mNumMeshes; ++MeshIdx) {
        aiMaterial* MeshMaterial = Scene->mMaterials[Scene->mMeshes[MeshIdx]->mMaterialIndex];
        Mesh CurrMesh(Scene->mMeshes[MeshIdx], MeshMaterial, mDirectory);
        mMeshes.push_back(CurrMesh);
        mMeshBuffers.push_back(Buffer(CurrMesh));

    }
    std::cout << "Loaded " << mMeshes.size() << " meshes" << std::endl;
    return true;
}

void
Model::Render(PhongShader &program) {
    mModel = glm::mat4(1.0f);
    mModel = glm::translate(mModel, mPosition);
    mModel = glm::rotate(mModel, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mModel = glm::rotate(mModel, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mModel = glm::rotate(mModel, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    mModel = glm::scale(mModel, mScale);

    for(unsigned MeshIdx = 0; MeshIdx < mMeshes.size(); ++MeshIdx) {
        Mesh &Mesh = mMeshes[MeshIdx];

        for(unsigned TexIdx = 0; TexIdx < Mesh.mTextures.size(); ++TexIdx) {
            Texture &Tex = Mesh.mTextures[TexIdx];
            // TODO(Jovan): Multiple same-type texture support
            program.BindTexture(Tex, TexIdx);
        }
        mMeshBuffers[MeshIdx].Render();
        for (auto Tex : Mesh.mTextures) {
            program.UnbindTexture(Tex);
        }
    }
}
