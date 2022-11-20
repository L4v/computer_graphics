/**
 * @file cubebuffer.hpp
 * @author Jovan Ivosevic
 * @brief Cube buffer implementation
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <vector>
#include "ibufferable.hpp"

class CubeBuffer : public IBufferable {
public:
    /**
     * @brief Empty ctor
     *
     */
    CubeBuffer();
private:
    static std::vector<float> mVertices;

    virtual float* GetVertices();
    virtual unsigned GetVertexCount();
    virtual unsigned GetVertexElementCount();
};