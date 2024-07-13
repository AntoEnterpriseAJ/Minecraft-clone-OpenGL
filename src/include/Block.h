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
	
	Block(Type type = Type::GRASS, float lengthOffset = 1, float widthOffset = 1, float heightOffset = 1);
	Type getType() const;
	std::vector<float> getVertices() const;
	//TYPES ARE FUCKED
private:
	float m_length, m_width, m_height;
	std::vector<float> m_vertices;
	Type m_type;
};