#pragma once
#include <vector>
#include "irenderobject.hpp"

class Quad : public IRenderObject {
private:
    static std::vector<float> mVertices;
    static std::vector<unsigned> mIndices;

public:
	Quad();
    virtual float* GetVertices();
    virtual unsigned* GetIndices();
    virtual unsigned GetVertexCount();
    virtual unsigned GetIndexCount();
    virtual unsigned GetVertexElementCount();
};