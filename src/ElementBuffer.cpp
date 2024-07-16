#include "include/ElementBuffer.h"
#include "include/VertexBuffer.h"

ElementBuffer::ElementBuffer()
	: m_count{0}
{
	glGenBuffers(1, &m_ID);
}

ElementBuffer::ElementBuffer(const void* data, unsigned int count)
	: m_count{count}
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

unsigned int ElementBuffer::getID() const
{
	return m_ID;
}

unsigned int ElementBuffer::getCount() const
{
	return m_count;
}

void ElementBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void ElementBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::setElements(const void* data, unsigned int count)
{
	m_count = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}
