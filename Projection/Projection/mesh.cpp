#include "mesh.hpp"

Mesh::Mesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath) {
    processMesh(mesh, MeshMaterial, resPath);
}

float*
Mesh::GetVertices() {
    return mVertices.data();
}

unsigned*
Mesh::GetIndices() {
    return mIndices.data();
}

unsigned
Mesh::GetVertexCount() {
    return mVertices.size();
}

unsigned
Mesh::GetIndexCount() {
    return mIndices.size();
}

void
Mesh::processMesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath) {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned VertexIndex = 0; VertexIndex < mesh->mNumVertices; ++VertexIndex) {
        std::vector<float> Position = { mesh->mVertices[VertexIndex].x, mesh->mVertices[VertexIndex].y, mesh->mVertices[VertexIndex].z };
        mVertices.insert(mVertices.end(), Position.begin(), Position.end());
    }

    for (unsigned FaceIndex = 0; FaceIndex < mesh->mNumFaces; ++FaceIndex) {
        const aiFace& Face = mesh->mFaces[FaceIndex];
        mIndices.push_back(Face.mIndices[0]);
        mIndices.push_back(Face.mIndices[1]);
        mIndices.push_back(Face.mIndices[2]);
    }
}