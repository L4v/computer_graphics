#pragma once
#include <vector>
#include "shader.hpp"

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
    std::vector<PointLight> mPointLights;
public:
    PhongShader(const std::string& vShaderPath, const std::string& fShaderPath);
    void AddPointLight(const PointLight& light);
    void BindTexture(const std::string& uniform, unsigned unitIdx, GLenum target, Texture& texture) const;
    void BindTexture(Texture& texture, GLenum unitIdx = 0) const;
    void BindDiffuse(Texture& texture, GLenum unitIdx = 0) const;
    void BindSpecular(Texture& texture, GLenum unitIdx = 0) const;
    void SetViewPosition(const glm::vec3& position) const;
};