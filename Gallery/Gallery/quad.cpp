#include "quad.hpp"

std::vector<float> Quad::mVertices = {
	//      POS        |       NORM      |   TEX
	-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

std::vector<unsigned> Quad::mIndices = {
		0, 1, 2,
		2, 1, 3
};

Quad::Quad() {}

float* Quad::GetVertices() {
	return mVertices.data();
}

unsigned* Quad::GetIndices() {
	return mIndices.data();
}

unsigned Quad::GetVertexCount() {
	return mVertices.size();
}

unsigned Quad::GetIndexCount() {
	return mIndices.size();
}

unsigned Quad::GetVertexElementCount() {
	return 8;
}
