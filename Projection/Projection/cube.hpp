#pragma once
#include <vector>

#include "irenderable.hpp"
#include "cubebuffer.hpp"
#include "buffer.hpp"

class Cube : public IRenderable {
public:
    Cube();
    void Render();
private:
    Buffer* mBuffer;

};