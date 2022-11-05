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
        const aiVector3D& aiTexCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][VertexIndex] : Zero3D;
        std::vector<float> Position = { mesh->mVertices[VertexIndex].x, mesh->mVertices[VertexIndex].y, mesh->mVertices[VertexIndex].z };
        mVertices.insert(mVertices.end(), Position.begin(), Position.end());
        std::vector<float> TexCoords = { aiTexCoord.x, aiTexCoord.y };
        mVertices.insert(mVertices.end(), TexCoords.begin(), TexCoords.end());
        std::vector<float> Normals = { mesh->mNormals[VertexIndex].x, mesh->mNormals[VertexIndex].y, mesh->mNormals[VertexIndex].z };
        mVertices.insert(mVertices.end(), Normals.begin(), Normals.end());
    }

    for (unsigned FaceIndex = 0; FaceIndex < mesh->mNumFaces; ++FaceIndex) {
        const aiFace& Face = mesh->mFaces[FaceIndex];
        mIndices.push_back(Face.mIndices[0]);
        mIndices.push_back(Face.mIndices[1]);
        mIndices.push_back(Face.mIndices[2]);
    }

    if (mesh->mMaterialIndex >= 0) {
        std::cout << MeshMaterial->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
        std::cout << MeshMaterial->GetTextureCount(aiTextureType_SPECULAR) << std::endl;
        loadTextures(MeshMaterial, Texture::DIFFUSE, resPath);
        loadTextures(MeshMaterial, Texture::SPECULAR, resPath);
    }
}

void
Mesh::loadTextures(aiMaterial* material, Texture::Type type, const std::string& dir) {
    // TODO(Jovan): A Map would be better; Cache textures outside of mesh class?
    aiTextureType aiType = type == Texture::DIFFUSE ? aiTextureType_DIFFUSE : aiTextureType_SPECULAR;
    for (unsigned TexIdx = 0; TexIdx < material->GetTextureCount(aiType); ++TexIdx) {
        aiString TexName;
        material->GetTexture(aiType, TexIdx, &TexName);
        // TODO(Jovan): Check for duplicates
        Texture Tex(dir + '/' + TexName.C_Str(), type);
        mTextures.push_back(Tex);
    }
}