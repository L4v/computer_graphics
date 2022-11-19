#pragma once

#include <vector>
#include "ibufferable.hpp"

class CubeBuffer : public IBufferable {
public:
    CubeBuffer();
private:
    static std::vector<float> mVertices;
    virtual float* GetVertices();
    virtual unsigned GetVertexCount();
    virtual unsigned GetVertexElementCount();
};