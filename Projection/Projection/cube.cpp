#include "cube.hpp"
#include "cubebuffer.hpp"

Cube::Cube() {
    CubeBuffer CubeBuffer;
    mBuffer = new Buffer(CubeBuffer);
}

void
Cube::Render() {
    mBuffer->Render();
}