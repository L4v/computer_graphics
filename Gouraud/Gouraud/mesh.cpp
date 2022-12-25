#include "mesh.hpp"

Mesh::Mesh(const aiMesh* mesh) {
    processMesh(mesh);
}

void
Mesh::Render() const {
    glBindVertexArray(mVAO);

    if (mIndexCount) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, (void*)0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return;
    }

    glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
    glBindVertexArray(0);
}

void
Mesh::processMesh(const aiMesh* mesh) {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned VertexIndex = 0; VertexIndex < mesh->mNumVertices; ++VertexIndex) {
        std::vector<float> Position = { mesh->mVertices[VertexIndex].x, mesh->mVertices[VertexIndex].y, mesh->mVertices[VertexIndex].z };
        mVertices.insert(mVertices.end(), Position.begin(), Position.end());
        std::vector<float> Normals = { mesh->mNormals[VertexIndex].x, mesh->mNormals[VertexIndex].y, mesh->mNormals[VertexIndex].z };
        mVertices.insert(mVertices.end(), Normals.begin(), Normals.end());
    }

    for (unsigned FaceIndex = 0; FaceIndex < mesh->mNumFaces; ++FaceIndex) {
        const aiFace& Face = mesh->mFaces[FaceIndex];
        mIndices.push_back(Face.mIndices[0]);
        mIndices.push_back(Face.mIndices[1]);
        mIndices.push_back(Face.mIndices[2]);
    }

    mVertexCount = mVertices.size() / 6;
    mIndexCount = mIndices.size();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (mIndexCount) {
        glGenBuffers(1, &mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexCount * sizeof(float), mIndices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
}