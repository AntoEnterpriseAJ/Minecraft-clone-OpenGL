#pragma once

#include <vector>
#include <unordered_map>
#include <array>

class Block
{
public:
	using uint = unsigned int;

	enum class Face
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		COUNT,
	};

	enum class Type
	{
		AIR     = 0,
		GRASS	= 1,
	};

	Block(Type type = Type::GRASS, int posX = 0, int posY = 0, int posZ = 0);

	bool isFaceVisible(Face face, int x, int y, int z, const std::vector<std::vector<std::vector<Block>>>& blocks) const;
	Type getType() const;
	std::vector<float> getFaceVertices(Face face) const;


private:
	void genFaceUV(Face face, float bottomLeftX, float bottomLeftY);
	void genUVs();
	bool isMeshGenerated() const;

	int m_posX, m_posY, m_posZ;
	std::unordered_map<Face, std::vector<float>> m_faceVertices;
	std::unordered_map<Face, std::vector<float>> m_UVs;
	bool m_meshGenerated;
	Type m_type;
};