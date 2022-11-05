#pragma once
#include <string>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>

class Texture {
public:
    enum Type {
        DIFFUSE = 0,
        SPECULAR,

        TYPECOUNT
    };

    Texture(const std::string& path, Type type = DIFFUSE,  float scale = 1.0f);
    ~Texture();
    const std::string& GetPath() const;
    void SetScale(float scale);
    float GetScale() const;
    unsigned GetId() const;
    Type GetType() const;
private:
    unsigned mId;
    std::string  mPath;
    GLenum mTarget;
    float mScale;
    Type mType;
};