#include "renderable.hpp"
int Renderable::rCount;

Renderable::Renderable(const float* vertices, const unsigned int verticesSize, const  unsigned int* indices, const int indicesSize) {
	vCount = verticesSize / (6 * sizeof(float));
	iCount = indicesSize / 3;

	
	glGenVertexArrays(1, &VAO);
	std::cout << "-Made an array-" << std::endl;
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::cout << "-Made a buffer-" << std::endl;
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, (6 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (iCount > 0)
	{
		std::cout << "-Made a buffer for indexing-" << std::endl;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);

	Renderable:rCount++;
}
Renderable::~Renderable() {

	glDeleteBuffers(1, &VBO);
	std::cout << "-Deleted a buffer-" << std::endl;
	if (iCount > 0){
		glDeleteBuffers(1, &EBO);
		std::cout << "-Deleted a buffer for indexing-" << std::endl;
	}
	std::cout << "-Deleted an array-" << std::endl;
	glDeleteVertexArrays(1, &VAO);

	Renderable::rCount--;
}
void Renderable::Render() {
	glBindVertexArray(VAO);
	if (iCount > 0)
	{
		std::cout << "-Drawing with indices-" << std::endl;
		glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		std::cout << "-Drawing with vertices-" << std::endl;
		glDrawArrays(GL_TRIANGLES, 0, vCount);
	}
	glBindVertexArray(0);
}