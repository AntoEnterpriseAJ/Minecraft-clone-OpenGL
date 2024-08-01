#include "include/Texture.h"
#include <iostream>
#include "glad/glad.h"

#include "stb_image.h"

Texture::Texture(const std::string& imagePath)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* imageData = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);

	if (!imageData)
	{
		std::cout << "WARNING: couldn't load image data at path: " << imagePath << std::endl;
	}

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
