/**
 * @file shader.cpp
 * @author Jovan Ivosevic
 * @brief Shader wrapper class
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
public:
    static const unsigned POSITION_LOCATION = 0;
    static const unsigned COLOR_LOCATION = 1;
    unsigned mId;

    Shader(const std::string& vShaderPath, const std::string& fShaderPath);
    unsigned GetId() const;

    /**
     * @brief Sets int uniform value
     *
     * @param uniform Name of uniform
     * @param v Value
     */
    void SetUniform1i(const std::string& uniform, int v) const;

    /**
     * @brief Sets float uniform value
     *
     * @param uniform Name of uniform
     * @param v Value
     */
    void SetUniform1f(const std::string& uniform, float v) const;

    /**
    * @brief Sets float uniform value
    *
    * @param uniform Name of uniform
    * @param v Value
    */
    void SetUniform3f(const std::string& uniform, const glm::vec3& v) const;

    /**
     * @brief Sets 4x4 matrix uniform value
     *
     * @param uniform Name of uniform
     * @param m GLM matrix
     */
    void SetUniform4m(const std::string& uniform, const glm::mat4& m) const;

    /**
     * @brief Sets the Model matrix
     *
     * @param m Model matrix
     */
    void SetModel(const glm::mat4& m) const;

    /**
     * @brief Sets the View matrix
     *
     * @param m View matrix
     */
    void SetView(const glm::mat4& m) const;

    /**
     * @brief Sets the Projection matrix
     *
     * @param m Projection matrix
     */
    void SetProjection(const glm::mat4& m) const;
private:

    /**
     * @brief Loads shader from file and returns the compiled shader's ID
     *
     * @param filename File path to be loaded
     * @param shadertType Type of shader: vertex or fragment
     * 
     * @returns Compiled shader's ID
     */
    unsigned loadAndCompileShader(std::string filename, GLuint shaderType);
    /**
     * @brief Creates a shader program and returns the ID
     *
     * @param filename File path to be loaded
     * @param shadertType Type of shader: vertex or fragment
     * 
     * @returns Shader program ID
     */
    unsigned createBasicProgram(unsigned vShader, unsigned fShader);
};