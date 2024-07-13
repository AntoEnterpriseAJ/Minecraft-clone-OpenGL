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
	
	Type getType() const;
	std::vector<float> getVertices() const;

	//TODO: QUESTIONABLE TYPE NAMING
	Block(Type type = Type::GRASS, float lengthOffset = 1, float widthOffset = 1, float heightOffset = 1);
private:
	float m_lengthOffset, m_widthOffset, m_heightOffset;
	std::vector<float> m_vertices;
	Type m_type;
};