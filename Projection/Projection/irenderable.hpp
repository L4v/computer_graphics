/**
 * @file irenderable.hpp
 * @author Jovan Ivosevic
 * @brief API for renderable objects
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

class IRenderable {
public:
    virtual void Render() = 0;
};