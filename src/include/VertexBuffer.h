#pragma once
#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void setVertices(const void* data, unsigned int size);
	unsigned int getID() const;
	void bind() const;
	void unbind() const;
private:
	unsigned int m_ID;
};
