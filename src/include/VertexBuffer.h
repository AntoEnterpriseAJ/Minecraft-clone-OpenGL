#pragma once
#include <glad/glad.h>

class VertexBuffer
{
public:
	void setVertices(const void* data, unsigned int size);

	unsigned int getID() const;
	void bind() const;
	void unbind() const;

	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
private:
	unsigned int m_ID;
};
