#pragma once

#include <vector>
#include <array>

class Block
{
public:
	enum Face
	{
		FRONT	= 0,
		BACK	= 1,
		LEFT	= 2,
		RIGHT	= 3,
		TOP		= 4,
		BOTTOM	= 5,
		COUNT	= 6,
	};

	enum class Type
	{
		AIR     = 0,
		GRASS	= 1,
		WATER   = 2,
	};

	Block();
	Block(Type type, int posX, int posY, int posZ);

	bool isFaceVisible(Face face, int x, int y, int z, const std::vector<Block>& blocks) const;
	Type getType() const;
	std::vector<float> getFaceVertices(Face face) const;

private:
	void genFaceUV(Face face, float bottomLeftX, float bottomLeftY);
	void genUVs();

	static constexpr int s_verticesPerFace = 20;
	static constexpr int s_UVsPerFace = 8;

	int m_posX, m_posY, m_posZ;
	std::array<float, Face::COUNT * s_verticesPerFace> m_faceVertices;
	std::array<float, Face::COUNT * s_UVsPerFace> m_UVs;

	Type m_type;
};