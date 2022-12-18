#include "shader.hpp"

Shader::Shader(const std::string& vShaderPath, const std::string& fShaderPath) {
    unsigned vs = loadAndCompileShader(vShaderPath, GL_VERTEX_SHADER);
    unsigned fs = loadAndCompileShader(fShaderPath, GL_FRAGMENT_SHADER);
    mId = createBasicProgram(vs, fs);
}

unsigned
Shader::GetId() const {
    return mId;
}

void
Shader::SetUniform1i(const std::string& uniform, int v) const {
    glUniform1i(glGetUniformLocation(mId, uniform.c_str()), v);
}

void
Shader::SetUniform1f(const std::string& uniform, float v) const {
    glUniform1f(glGetUniformLocation(mId, uniform.c_str()), v);
}

void
Shader::SetUniform4m(const std::string& uniform, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(mId, uniform.c_str()), 1, GL_FALSE, &m[0][0]);
}

void
Shader::SetModel(const glm::mat4& m) const {
    SetUniform4m("uModel", m);
}

void
Shader::SetView(const glm::mat4& m) const {
    SetUniform4m("uView", m);
}

void Shader::SetProjection(const glm::mat4& m) const {
    SetUniform4m("uProjection", m);
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