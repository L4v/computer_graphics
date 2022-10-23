#pragma once
#include <vector>
#include "irenderobject.hpp"

class Cube : public IRenderObject {
private:
    static std::vector<float> mVertices;
    static std::vector<unsigned> mIndices;
public:
    Cube();
    virtual float* GetVertices();
    virtual unsigned GetVertexCount();
    virtual unsigned GetVertexElementCount();
};