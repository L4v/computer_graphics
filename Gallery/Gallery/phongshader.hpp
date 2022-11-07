#pragma once
#include "shader.hpp"
#include <vector>
#include <map>

class PointLight {
public:
    glm::vec3 mPosition;

    glm::vec3 mKa;
    glm::vec3 mKd;
    glm::vec3 mKs;

    float mKc;
    float mKl;
    float mKq;

    PointLight(const glm::vec3& position, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks) {
        mPosition = position;
        mKa = ka;
        mKd = kd;
        mKs = ks;

        mKc = 1.0f;
        mKl = 0.022f;
        mKq = 0.0019f;
    }

};

class PhongShader : public Shader {
private:
    struct BoundTextureInfo {
        GLenum Target;
        unsigned Unit;
    };

    std::vector<PointLight> mPointLights;
    std::map<unsigned, BoundTextureInfo> mBoundTextures;
public:
    PhongShader(const std::string& vShaderPath, const std::string& fShaderPath);
    void AddPointLight(const PointLight& light);
    void BindTexture(const std::string& uniform, unsigned unitIdx, GLenum target, Texture& texture);
    void BindTexture(Texture& texture, unsigned unitIdx = 0);
    void BindDiffuse(Texture& texture, unsigned unitIdx = 0);
    void BindSpecular(Texture& texture, unsigned unitIdx = 0);
    void SetViewPosition(const glm::vec3& position) const;
    void UnbindTexture(Texture& texture);
};