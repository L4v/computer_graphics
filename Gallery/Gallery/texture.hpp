#pragma once
#include <string>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();
    const std::string& GetPath() const;
    void SetScale(float scale);
    float GetScale() const;
    unsigned GetId() const;
private:
    unsigned mId;
    std::string  mPath;
    GLenum mTarget;
    float mScale;

};