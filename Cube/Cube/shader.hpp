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
    Shader(const std::string& vShaderPath, const std::string& gShaderPath, const std::string& fShaderPath);
    unsigned GetId() const;
    void SetUniform4m(const std::string& uniform, const glm::mat4& m) const;
private:
    unsigned loadAndCompileShader(std::string filename, GLuint shaderType);
    unsigned createBasicProgram(unsigned vShader, unsigned fShader);
    unsigned createBasicProgram(unsigned vShader, unsigned gShader, unsigned fShader);
};