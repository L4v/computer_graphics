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

    /**
     * @brief Ctor
     *
     * @param vShaderPath Vertex shader file path
     * @param fShaderPath Fragment shader file path
     */
    Shader(const std::string& vShaderPath, const std::string& fShaderPath);

    /**
     * @brief Gets shader ID
     *
     * @returns Shader ID
     */
    unsigned GetId() const;

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
    unsigned mId;

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