#include "buffer.hpp"
#include <iostream>

Buffer::Buffer(IRenderObject& renderObject) : mEBO(0) {
    mIndexCount = renderObject.GetIndexCount();
    mVertexCount = renderObject.GetVertexCount() / renderObject.GetVertexElementCount();
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, renderObject.GetVertexCount() * sizeof(float), renderObject.GetVertices(), GL_STATIC_DRAW);

    float Stride = renderObject.GetVertexElementCount() * sizeof(float);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, Stride, (void*)0);
    glEnableVertexAttribArray(POSITION_LOCATION);

    glVertexAttribPointer(COLOUR_LOCATION, 3, GL_FLOAT, GL_FALSE, Stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(COLOUR_LOCATION);

    if (mIndexCount) {
        glGenBuffers(1, &mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderObject.GetIndexCount() * sizeof(float), renderObject.GetIndices(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void
Buffer::Render() {
    glBindVertexArray(mVAO);
    if (mIndexCount) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, (void*)0);
        return;
    }
    glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
}
