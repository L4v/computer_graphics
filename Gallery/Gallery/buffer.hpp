#pragma once
#include <GL/glew.h>
#include "irenderobject.hpp"

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2

class Buffer {
private:
	unsigned mVAO;
	unsigned mVBO;
	unsigned mEBO;
	unsigned mIndexCount;
	unsigned mVertexCount;
public:
	Buffer(IRenderObject& renderObject, int stride = 8 * sizeof(float));
	void Render();
};