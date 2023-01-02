#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned
Texture::LoadImageToTexture(const std::string& filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    std::cout << "Loading texture: " << filePath << std::endl;
    unsigned char* ImageData = stbi_load(filePath.c_str(), &TextureWidth, &TextureHeight, &TextureChannels, 0);

    if (!ImageData) {
        std::cerr << "Failed to load texture: " << filePath << " loading default instead" << std::endl;
        return LoadImageToTexture(MISSING_TEXTURE_PATH);
    }

    // NOTE(Jovan): Images should usually flipped vertically as they are loaded "upside-down"
    stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

    // NOTE(Jovan): Checks or "guesses" the loaded image's format
    GLint InternalFormat = -1;
    switch (TextureChannels) {
    case 1: InternalFormat = GL_RED; break;
    case 3: InternalFormat = GL_RGB; break;
    case 4: InternalFormat = GL_RGBA; break;
    default: InternalFormat = GL_RGB; break;
    }

    unsigned Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    // NOTE(Jovan): ImageData is no longer necessary in RAM and can be deallocated
    stbi_image_free(ImageData);
    return Texture;
}