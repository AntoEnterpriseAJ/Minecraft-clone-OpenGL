#pragma once

#include  <vector>
#include <windows.h>
#include <glad/glad.h>

class VertexBufferLayout
{
public:
	struct layout
	{
		unsigned int type;
		unsigned int count; //2, 3, 4
		bool normalized;

		unsigned int getTypeSize()
		{
			switch (type)
			{
				case GL_FLOAT			: return sizeof(GL_FLOAT);
				case GL_UNSIGNED_INT	: return sizeof(GL_UNSIGNED_INT);
				case GL_UNSIGNED_BYTE	: return sizeof(GL_UNSIGNED_BYTE);
			}
		}
	};

	VertexBufferLayout() : m_stride{0} {}
	~VertexBufferLayout() = default;

	void addLayout(unsigned int type, unsigned int count, bool normalized)
	{
		m_layouts.push_back({type, count, normalized});
		m_stride += count * m_layouts.back().getTypeSize();
	}
	unsigned int getCount() const {return m_layouts.size();}
	layout getLayoutAt(unsigned int index) const {return m_layouts[index];}
	unsigned int getStride() const {return m_stride;}
private:


	unsigned int m_stride;
	std::vector<layout> m_layouts;
};
