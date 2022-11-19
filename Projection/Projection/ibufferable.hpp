#pragma once
class IBufferable {
public:
	virtual float* GetVertices() = 0;
	virtual unsigned* GetIndices() {
		return 0;
	};
	virtual unsigned GetVertexCount() = 0;
	virtual unsigned GetIndexCount() {
		return 0;
	};
	virtual unsigned GetVertexElementCount() {
		return 6;
	};
};