#pragma once

#include "include/VertexBuffer.h"
#include "include/VertexBufferLayout.h"

class VertexArray
{
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbLayout);

	void bind() const;
	void unbind() const;
private:
	unsigned int m_ID;
};