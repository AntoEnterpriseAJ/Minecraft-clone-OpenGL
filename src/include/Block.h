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
		LOG     = 3,
		LEAVES  = 4,
		DIRT    = 5,
		STONE	= 6,
		COAL    = 7,
	};

	Block();
	Block(Type type);

	bool isFaceVisible(Face face, int x, int y, int z, const std::vector<Block>& blocks) const;
	Type getType() const;
	float getUV(Face face, int index) const;

	void setType(Block::Type type);

private:
	void genFaceUV(Face face, float bottomLeftX, float bottomLeftY);

	static constexpr int s_UVsPerFace = 8;
	std::array<float, Face::COUNT * s_UVsPerFace> m_UVs;
	Type m_type;
};