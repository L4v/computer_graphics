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
PhongShader::BindTexture(const std::string& uniform, unsigned unitIdx, GLenum target, Texture& texture) {
    glActiveTexture(GL_TEXTURE0 + unitIdx);
    SetUniform1i(uniform, unitIdx);
    SetUniform1f("uTexScale", texture.GetScale());
    glBindTexture(target, texture.GetId());
    mBoundTextures[texture.GetId()] = { target, unitIdx };
}

void
PhongShader::BindTexture(Texture& texture, unsigned unitIdx) {
    if (texture.GetType() == Texture::SPECULAR) {
        return BindSpecular(texture, unitIdx);
    }
    BindDiffuse(texture, unitIdx);
}
void
PhongShader::BindDiffuse(Texture& texture, unsigned unitIdx) {
    BindTexture("uDiffuse", unitIdx, GL_TEXTURE_2D, texture);
}

void
PhongShader::BindSpecular(Texture& texture, unsigned unitIdx) {
    BindTexture("uSpecular", unitIdx, GL_TEXTURE_2D, texture);
}


void
PhongShader::SetViewPosition(const glm::vec3& position) const {
    SetUniform3f("uViewPos", position);
}

void 
PhongShader::UnbindTexture(Texture& texture) {
    std::map<unsigned, BoundTextureInfo>::const_iterator It = mBoundTextures.find(texture.GetId());
    if (It != mBoundTextures.end()) {
        BoundTextureInfo Info = It->second;
        glActiveTexture(GL_TEXTURE0 + Info.Unit);
        glBindTexture(Info.Target, 0);
    }
}