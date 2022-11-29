#include "mesh.hpp"

Mesh::Mesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath)
    : mVerticesCount(0), mIndicesCount(0){
    processMesh(mesh, MeshMaterial, resPath);
}

void
Mesh::Render() const {
    glBindVertexArray(mVAO);
    if(mIndicesCount) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(mVAO);
        return;
    }
    glDrawArrays(GL_TRIANGLES, 0, mVerticesCount);
    glBindVertexArray(0);
}

void
Mesh::processMesh(const aiMesh* mesh, aiMaterial* MeshMaterial, const std::string& resPath) {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    std::vector<float> Vertices;
    for (unsigned VertexIndex = 0; VertexIndex < mesh->mNumVertices; ++VertexIndex) {
        std::vector<float> Position = { mesh->mVertices[VertexIndex].x, mesh->mVertices[VertexIndex].y, mesh->mVertices[VertexIndex].z };
        Vertices.insert(Vertices.end(), Position.begin(), Position.end());
        //Upotreba normala za boje
        std::vector<float> Normals = { mesh->mNormals[VertexIndex].x, mesh->mNormals[VertexIndex].y, mesh->mNormals[VertexIndex].z };
        Vertices.insert(Vertices.end(), Normals.begin(), Normals.end());
        //aiColor4D Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        // NOTE(Jovan): If material isn't being rendered properly
        // comment out the line below
        //aiGetMaterialColor(MeshMaterial, AI_MATKEY_COLOR_DIFFUSE, &Color); // <-- This one
        //std::vector<float> VertexColor = { Color.r, Color.g, Color.b };
        //Vertices.insert(Vertices.end(), VertexColor.begin(), VertexColor.end());


    }

    mVerticesCount = Vertices.size();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVerticesCount * sizeof(float), Vertices.data(), GL_STATIC_DRAW);
    float Stride = 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::vector<unsigned> Indices;
    for (unsigned FaceIndex = 0; FaceIndex < mesh->mNumFaces; ++FaceIndex) {
        const aiFace& Face = mesh->mFaces[FaceIndex];
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
    mIndicesCount = Indices.size();

    if (mIndicesCount) {
        glGenBuffers(1, &mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(float), Indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
}