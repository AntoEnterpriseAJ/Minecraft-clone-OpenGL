#pragma once

#include <vector>
#include <array>
#include <memory>

class Block
{
public:
    enum Face
    {
        FRONT   = 0,
        BACK    = 1,
        LEFT    = 2,
        RIGHT   = 3,
        TOP     = 4,
        BOTTOM  = 5,
        COUNT   = 6,
    };

    enum class Type
    {
        AIR     = 0,
        GRASS   = 1,
        WATER   = 2,
        LOG     = 3,
        LEAVES  = 4,
        DIRT    = 5,
        STONE   = 6,
        COAL    = 7,
    };

    Block();
    Block(Type type);
    Block(const Block& other);
    Block& operator=(const Block& other);

    bool isFaceVisible(Face face, int x, int y, int z, const std::vector<Block>& blocks) const;
    Type getType() const;
    float getUV(Face face, int index) const;

    void setType(Block::Type type);

    static constexpr std::array<float, 18> s_faceNormals = {
        	0.0f,  0.0f,  1.0f, // FRONT
		0.0f,  0.0f, -1.0f, // BACK
	       -1.0f,  0.0f,  0.0f, // LEFT
		1.0f,  0.0f,  0.0f, // RIGHT
		0.0f,  1.0f,  0.0f, // TOP
		0.0f, -1.0f,  0.0f, // BOTTOM
	};

private:
    void genUVs();
    void genFaceUV(Face face, float bottomLeftX, float bottomLeftY);
private:
    static constexpr int s_UVsPerFace = 8;
private:
    std::unique_ptr<std::array<float, Face::COUNT * s_UVsPerFace>> m_UVs;
    Type m_type;
};
