#include "model.hpp"

Model::Model(std::string filename) {
    mFilename = filename;
    mDirectory = filename.substr(0, filename.find_last_of('/'));
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

    }
    std::cout << mFilename << " Loaded " << mMeshes.size() << " meshes" << std::endl;
    return true;
}

void
Model::Render() {
    for (const Mesh& mesh : mMeshes) {
        mesh.Render();
    }
}
