/**
 * @file buffer.hpp
 * @author Jovan Ivosevic
 * @brief GL Buffer abstraction
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */

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
	/**
	 * @brief Ctor - buffers Bufferable Object data into GL
	 *
	 * @param bufferable Bufferable object
	 */
	Buffer(IBufferable& bufferable);
	/**
	 * @brief Renders the buffered object
	 *
	 */
	void Render();
};