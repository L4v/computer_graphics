#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

Texture::Texture(const std::string& path)
    : mPath(path), mScale(8.0f) {
    std::replace(mPath.begin(), mPath.end(), '\\', '/');
    glGenTextures(1, &mId);
    int Width, Height, Channels;
    unsigned char* Data = stbi_load(mPath.c_str(), &Width, &Height, &Channels, 0);
    std::cout << "Loading " << mPath << " image" << std::endl;
    if (Data) {
        GLenum Format = 0;
        switch (Channels) {
        case 1: Format = GL_RED; break;
        case 3: Format = GL_RGB; break;
        default: Format = GL_RGBA; break;
        }

        glBindTexture(GL_TEXTURE_2D, mId);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        stbi_image_free(Data);
    }
    else {
        std::cerr << "[Err] Texture failed to load" << std::endl;
    }
}

Texture::~Texture() {}

const std::string& 
Texture::GetPath() const {
    return mPath;
}

void
Texture::SetScale(float scale) {
    mScale = scale;
}

float
Texture::GetScale() const {
    return mScale;
}

unsigned
Texture::GetId() const {
    return mId;
}