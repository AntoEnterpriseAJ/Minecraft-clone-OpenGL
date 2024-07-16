#pragma once

#include <vector>

class Block
{
public:
	using uint = unsigned int;
	enum class Type
	{
		AIR     = 0,
		GRASS	= 1,
	};

	Block(Type type = Type::GRASS, int posX = 0, int posY = 0, int posZ = 0);
	
	void addFace(const std::vector<float>& faceVertices, const std::vector<unsigned int>& indices);
	void generateVertices();

	Type getType() const;
	std::vector<float> getVertices() const;
	std::vector<unsigned int> getIndices() const;

private:
	int m_posX, m_posY, m_posZ;
	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;
	Type m_type;
};