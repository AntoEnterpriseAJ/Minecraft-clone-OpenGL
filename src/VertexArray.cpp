#include  "include/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbLayout)
{
	bind();
	vb.bind();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < vbLayout.getCount(); i++)
	{
		VertexBufferLayout::layout layout = vbLayout.getLayoutAt(i);

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, layout.count, layout.type, layout.normalized, vbLayout.getStride(), (void*)(offset));

		offset += layout.count * layout.getTypeSize();
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
