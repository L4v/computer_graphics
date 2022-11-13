#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>

class Shader {
public:

    Shader(const std::string& vShaderPath, const std::string& fShaderPath);
    unsigned GetId() const;
private:
    unsigned mId;
    unsigned loadAndCompileShader(std::string filename, GLuint shaderType);
    unsigned createBasicProgram(unsigned vShader, unsigned fShader);
};