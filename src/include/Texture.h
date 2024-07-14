#pragma once

class Texture
{
public:
	Texture(unsigned char* image, int width, int height);

	void bind() const;
	void unbind() const;
private:
	unsigned int m_ID;
};