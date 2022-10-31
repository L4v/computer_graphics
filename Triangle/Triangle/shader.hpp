#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>

class Shader {
public:
    unsigned mId;

    Shader(const std::string& vShaderPath, const std::string& fShaderPath);
    Shader(const std::string& vShaderPath, const std::string& gShaderPath, const std::string& fShaderPath);
private:
    unsigned loadAndCompileShader(std::string filename, GLuint shaderType);
    unsigned createBasicProgram(unsigned vShader, unsigned fShader);
    unsigned createBasicProgram(unsigned vShader, unsigned gShader, unsigned fShader);
};