#include "shader.hpp"

ShaderProgram::ShaderProgram(const std::string& vShaderPath, const std::string& fShaderPath) {
    u32 vs = loadAndCompileShader(vShaderPath, GL_VERTEX_SHADER);
    u32 fs = loadAndCompileShader(fShaderPath, GL_FRAGMENT_SHADER);
    mId = createBasicProgram(vs, fs);
}

ShaderProgram::ShaderProgram(const std::string& vShaderPath, const std::string& gShaderPath, const std::string& fShaderPath) {
    u32 vs = loadAndCompileShader(vShaderPath, GL_VERTEX_SHADER);
    u32 gs = loadAndCompileShader(gShaderPath, GL_GEOMETRY_SHADER);
    u32 fs = loadAndCompileShader(fShaderPath, GL_FRAGMENT_SHADER);
    mId = createBasicProgram(vs, gs, fs);
}

u32
ShaderProgram::loadAndCompileShader(std::string filename, GLuint shaderType) {
    u32 ShaderID = 0;
    std::ifstream In(filename);
    std::string Str;

    In.seekg(0, std::ios::end);
    Str.reserve(In.tellg());
    In.seekg(0, std::ios::beg);

    Str.assign((std::istreambuf_iterator<char>(In)), std::istreambuf_iterator<char>());
    const char *CharContent = Str.c_str();

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

    return ShaderID;
}

u32
ShaderProgram::createBasicProgram(u32 vShader, u32 fShader) {
    u32 ProgramID = 0;
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vShader);
    glAttachShader(ProgramID, fShader);
    glLinkProgram(ProgramID);

    i32 Success;
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

u32
ShaderProgram::createBasicProgram(u32 vShader, u32 gShader, u32 fShader) {
    u32 ProgramID = 0;
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vShader);
    glAttachShader(ProgramID, gShader);
    glAttachShader(ProgramID, fShader);
    glLinkProgram(ProgramID);

    i32 Success;
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

void
ShaderProgram::SetUniform1i(const std::string &uniform, i32 i) const {
    glUniform1i(glGetUniformLocation(mId, uniform.c_str()), i);
}


void
ShaderProgram::SetUniform1f(const std::string &uniform, r32 f) const {
    glUniform1f(glGetUniformLocation(mId, uniform.c_str()), f);
}

void
ShaderProgram::SetUniform2f(const std::string& uniform, const glm::vec2& v) const {
    glUniform2fv(glGetUniformLocation(mId, uniform.c_str()), 1, &v[0]);
}

void
ShaderProgram::SetUniform3f(const std::string &uniform, const glm::vec3 &v) const {
    glUniform3fv(glGetUniformLocation(mId, uniform.c_str()), 1, &v[0]);
}

void
ShaderProgram::SetUniform4m(const std::string &uniform, const glm::mat4 &m) const {
    glUniformMatrix4fv(glGetUniformLocation(mId, uniform.c_str()), 1, GL_FALSE, &m[0][0]);
}
