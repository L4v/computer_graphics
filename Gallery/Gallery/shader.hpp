#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "math3d.hpp"
#include "texture.hpp"

class Shader {
public:
    unsigned mId;

    Shader(const std::string& vShaderPath, const std::string& fShaderPath);
    Shader(const std::string& vShaderPath, const std::string& gShaderPath, const std::string& fShaderPath);
    void SetUniform1i(const std::string& uniform, int i) const;
    void SetUniform1f(const std::string& uniform, float f) const;
    void SetUniform2f(const std::string& uniform, const glm::vec2& v) const;
    void SetUniform3f(const std::string& uniform, const v3& v) const;
    void SetUniform3f(const std::string& uniform, const glm::vec3& v) const;
    void SetUniform4m(const std::string& uniform, const m44& m, GLboolean transpose = GL_FALSE) const;
    void SetUniform4m(const std::string& uniform, const std::vector<m44>& m, GLboolean transpose = GL_FALSE) const;
    void SetUniform4m(const std::string& uniform, const glm::mat4& m) const;
    void SetModel(const glm::mat4& m) const;
    void SetView(const glm::mat4& m) const;
    void SetProjection(const glm::mat4& m) const;
    void SetMVP(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const;
private:
    unsigned loadAndCompileShader(std::string filename, GLuint shaderType);
    unsigned createBasicProgram(unsigned vShader, unsigned fShader);
    unsigned createBasicProgram(unsigned vShader, unsigned gShader, unsigned fShader);
};