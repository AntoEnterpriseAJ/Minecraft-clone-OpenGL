#pragma once

#include <vector>
#include <unordered_map>
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
	};

	Block(Type type = Type::GRASS, int posX = 0, int posY = 0, int posZ = 0);

	bool isFaceVisible(Face face, int x, int y, int z, const std::vector<std::vector<std::vector<Block>>>& blocks) const;
	Type getType() const;
	std::vector<float> getFaceVertices(Face face) const;
	bool isMeshGenerated() const;


private:
	void genFaceUV(Face face, float bottomLeftX, float bottomLeftY);
	void genUVs();

	static constexpr int s_verticesPerFace = 20;
	static constexpr int s_UVsPerFace = 8;

	int m_posX, m_posY, m_posZ;
	std::array<float, Face::COUNT * s_verticesPerFace> m_faceVertices;
	std::array<float, Face::COUNT * s_UVsPerFace> m_UVs;

	bool m_meshGenerated;
	Type m_type;
};