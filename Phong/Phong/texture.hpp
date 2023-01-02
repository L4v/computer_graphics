#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>

static const std::string MISSING_TEXTURE_PATH = "res/missing_texture";

class Texture {
public:
	/**
	 * @brief Loads image file and creates an OpenGL texture.
	 * NOTE: Try avoiding .jpg and other lossy compression formats as
	 * they are uncompressed during loading and the memory benefit is
	 * negated with the addition of loss of quality
	 *
	 * @param filePath Image file path
	 * @returns TextureID
	 */
	static unsigned LoadImageToTexture(const std::string& filePath);
};
