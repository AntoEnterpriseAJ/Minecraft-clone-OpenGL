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

	Block(Type type = Type::GRASS, float lengthOffset = 0, float widthOffset = 0, float heightOffset = 0);
	
	void addFace(const std::vector<float>& faceVertices);
	void generateVertices();

	Type getType() const;
	std::vector<float> getVertices() const;

private:
	float m_lengthOffset, m_widthOffset, m_heightOffset;
	std::vector<float> m_vertices;
	Type m_type;
};