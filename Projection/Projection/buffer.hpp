#pragma once
#include <GL/glew.h>
#include <iostream>
#include "ibufferable.hpp"

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
	Buffer(IBufferable& renderObject);
	void Render();
};