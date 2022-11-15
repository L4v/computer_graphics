#pragma once
#include <GL/glew.h>
#include "irenderobject.hpp"

#define POSITION_LOCATION 0
#define COLOUR_LOCATION 1

class Buffer {
private:
	unsigned mVAO;
	unsigned mVBO;
	unsigned mEBO;
	unsigned mIndexCount;
	unsigned mVertexCount;
public:
	Buffer(IRenderObject& renderObject);
	void Render();
};