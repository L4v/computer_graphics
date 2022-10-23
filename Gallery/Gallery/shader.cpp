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

void
Shader::SetPointLight(const Light& light, unsigned index) {
    SetUniform3f("uPointLights[0].Ambient", light.Ambient);
    SetUniform3f("uPointLights[0].Diffuse", light.Diffuse);
    SetUniform3f("uPointLights[0].Specular", light.Specular);
    SetUniform3f("uPointLights[0].Position", light.Position);
    SetUniform1f("uPointLights[0].Kc", light.Kc);
    SetUniform1f("uPointLights[0].Kl", light.Kl);
    SetUniform1f("uPointLights[0].Kq", light.Kq);
}

void
Shader::SetUniform1i(const std::string& uniform, int i) const {
    glUniform1i(glGetUniformLocation(mId, uniform.c_str()), i);
}

void
Shader::SetUniform1f(const std::string& uniform, float f) const {
    glUniform1f(glGetUniformLocation(mId, uniform.c_str()), f);
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

void
Shader::SetMVP(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const {
    SetModel(m);
    SetView(v);
    SetProjection(p);
}

void
Shader::SetUniform2f(const std::string& uniform, const glm::vec2& v) const {
    glUniform2fv(glGetUniformLocation(mId, uniform.c_str()), 1, &v[0]);
}

void
Shader::SetUniform3f(const std::string& uniform, const v3& v) const {
    glUniform3fv(glGetUniformLocation(mId, uniform.c_str()), 1, &v[0]);
}

void
Shader::SetUniform3f(const std::string& uniform, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(mId, uniform.c_str()), 1, &v[0]);
}

void
Shader::SetUniform4m(const std::string& uniform, const m44& m, GLboolean transpose) const {
    glUniformMatrix4fv(glGetUniformLocation(mId, uniform.c_str()), 1, transpose, &m[0][0]);
}

void
Shader::SetUniform4m(const std::string& uniform, const std::vector<m44>& m, GLboolean transpose) const {
    glUniformMatrix4fv(glGetUniformLocation(mId, uniform.c_str()), m.size(), transpose, (GLfloat*)&m[0]);
}

void
Shader::SetUniform4m(const std::string& uniform, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(mId, uniform.c_str()), 1, GL_FALSE, &m[0][0]);
}

void
Shader::BindTexture(const std::string& uniform, unsigned unitIdx, GLenum target, Texture& texture) const {
    glActiveTexture(GL_TEXTURE0 + unitIdx);
    SetUniform1i(uniform, unitIdx);
    SetUniform1f("uTexScale", texture.GetScale());
    glBindTexture(target, texture.GetId());
}
void
Shader::BindDiffuse(Texture& texture, GLenum unitIdx) const {
    BindTexture("uDiffuse", unitIdx, GL_TEXTURE_2D, texture);
}

void
Shader::BindSpecular(Texture& texture, GLenum unitIdx) const {
    BindTexture("uSpecular", unitIdx, GL_TEXTURE_2D, texture);
}