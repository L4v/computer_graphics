#include "shader.hpp"


Shader::Shader(const std::string& vShaderPath, const std::string& fShaderPath) {
    unsigned vs = loadAndCompileShader(vShaderPath, GL_VERTEX_SHADER);
    unsigned fs = loadAndCompileShader(fShaderPath, GL_FRAGMENT_SHADER);
    mId = createBasicProgram(vs, fs);
}

Shader::Shader(const std::string& vShaderPath, const std::string& gShaderPath, const std::string& fShaderPath) {
    unsigned vs = loadAndCompileShader(vShaderPath, GL_VERTEX_SHADER);
    unsigned gs = loadAndCompileShader(gShaderPath, GL_GEOMETRY_SHADER);
    unsigned fs = loadAndCompileShader(fShaderPath, GL_FRAGMENT_SHADER);
    mId = createBasicProgram(vs, gs, fs);
}

unsigned
Shader::loadAndCompileShader(std::string filename, GLuint shaderType) {
    unsigned ShaderID = 0;
    std::ifstream In(filename);
    std::string Str;

    In.seekg(0, std::ios::end);
    Str.reserve(In.tellg());
    In.seekg(0, std::ios::beg);

    Str.assign((std::istreambuf_iterator<char>(In)), std::istreambuf_iterator<char>());
    const char* CharContent = Str.c_str();

    ShaderID = glCreateShader(shaderType);
    glShaderSource(ShaderID, 1, &CharContent, NULL);
    glCompileShader(ShaderID);

    int Success;
    char InfoLog[512];
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(ShaderID, 256, NULL, InfoLog);
        std::string ShaderTypeName = shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment";
        std::cout << "Error while compiling shader [" << ShaderTypeName << "]:" << std::endl << InfoLog << std::endl;
        return 0;
    }

    std::cout << "Loaded " << filename << " shader" << std::endl;

    return ShaderID;
}

unsigned
Shader::createBasicProgram(unsigned vShader, unsigned fShader) {
    unsigned ProgramID = 0;
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vShader);
    glAttachShader(ProgramID, fShader);
    glLinkProgram(ProgramID);

    int Success;
    char InfoLog[512];
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ProgramID, 512, NULL, InfoLog);
        std::cerr << "[Err] Failed to link shader program:" << std::endl << InfoLog << std::endl;
        return 0;
    }

    glDetachShader(ProgramID, vShader);
    glDetachShader(ProgramID, fShader);
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return ProgramID;
}

unsigned
Shader::createBasicProgram(unsigned vShader, unsigned gShader, unsigned fShader) {
    unsigned ProgramID = 0;
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vShader);
    glAttachShader(ProgramID, gShader);
    glAttachShader(ProgramID, fShader);
    glLinkProgram(ProgramID);

    int Success;
    char InfoLog[512];
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ProgramID, 512, NULL, InfoLog);
        std::cerr << "[Err] Failed to link shader program:" << std::endl << InfoLog << std::endl;
        return 0;
    }

    glDetachShader(ProgramID, vShader);
    glDetachShader(ProgramID, gShader);
    glDetachShader(ProgramID, fShader);
    glDeleteShader(vShader);
    glDeleteShader(gShader);
    glDeleteShader(fShader);

    return ProgramID;
}