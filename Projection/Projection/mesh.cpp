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
        // NOTE(Jovan): Normals are used as color data for now
        std::vector<float> Normals = { mesh->mNormals[VertexIndex].x, mesh->mNormals[VertexIndex].y, mesh->mNormals[VertexIndex].z };
        mVertices.insert(mVertices.end(), Normals.begin(), Normals.end());
        /*aiColor4D Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        aiGetMaterialColor(MeshMaterial, AI_MATKEY_COLOR_DIFFUSE, &Color);
        std::vector<float> VertexColor = { Color.r, Color.g, Color.b };
        mVertices.insert(mVertices.end(), VertexColor.begin(), VertexColor.end());*/
    }

    for (unsigned FaceIndex = 0; FaceIndex < mesh->mNumFaces; ++FaceIndex) {
        const aiFace& Face = mesh->mFaces[FaceIndex];
        mIndices.push_back(Face.mIndices[0]);
        mIndices.push_back(Face.mIndices[1]);
        mIndices.push_back(Face.mIndices[2]);
    }
}