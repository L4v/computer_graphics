#include "phongshader.hpp"

PhongShader::PhongShader(const std::string& vShaderPath, const std::string& fShaderPath)
    : Shader(vShaderPath, fShaderPath) {
}

void
PhongShader::AddPointLight(const PointLight& light) {
    unsigned PointLightIdx = mPointLights.size();
    mPointLights.push_back(light);
    SetUniform1i("uPointLightCount", mPointLights.size());
    SetUniform3f("uPointLights[" + std::to_string(PointLightIdx) + "].Position", light.mPosition);
    SetUniform3f("uPointLights[" + std::to_string(PointLightIdx) + "].Ka", light.mKa);
    SetUniform3f("uPointLights[" + std::to_string(PointLightIdx) + "].Kd", light.mKd);
    SetUniform3f("uPointLights[" + std::to_string(PointLightIdx) + "].Ks", light.mKs);
    SetUniform1f("uPointLights[" + std::to_string(PointLightIdx) + "].Kc", light.mKc);
    SetUniform1f("uPointLights[" + std::to_string(PointLightIdx) + "].Kl", light.mKl);
    SetUniform1f("uPointLights[" + std::to_string(PointLightIdx) + "].Kq", light.mKq);
}

void
PhongShader::BindTexture(const std::string& uniform, unsigned unitIdx, GLenum target, Texture& texture) const {
    glActiveTexture(GL_TEXTURE0 + unitIdx);
    SetUniform1i(uniform, unitIdx);
    SetUniform1f("uTexScale", texture.GetScale());
    glBindTexture(target, texture.GetId());
}

void
PhongShader::BindTexture(Texture& texture, GLenum unitIdx) const {
    if (texture.GetType() == Texture::SPECULAR) {
        return BindSpecular(texture, unitIdx);
    }
    BindDiffuse(texture, unitIdx);
}
void
PhongShader::BindDiffuse(Texture& texture, GLenum unitIdx) const {
    BindTexture("uDiffuse", unitIdx, GL_TEXTURE_2D, texture);
}

void
PhongShader::BindSpecular(Texture& texture, GLenum unitIdx) const {
    BindTexture("uSpecular", unitIdx, GL_TEXTURE_2D, texture);
}


void
PhongShader::SetViewPosition(const glm::vec3& position) const {
    SetUniform3f("uViewPos", position);
}
