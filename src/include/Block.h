#pragma once

#include <vector>

class Block
{
public:
	using uint = unsigned int;
	enum class Type
	{
		AIR		= 0,
		GRASS	= 1,
	};
	
	Block(Type type = Type::GRASS, uint length = 1, uint width = 1, uint height = 1);
	Type getType() const;
	std::vector<float> getVertices() const;

private:
	uint m_length, m_width, m_height;
	std::vector<float> m_vertices;
	Type m_type;
};