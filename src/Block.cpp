#include "include/Block.h"
#include "include/Chunk.h"

Block::Block()
    : m_type{Type::AIR}, m_UVs{nullptr} {}

Block::Block(Type type)
    : m_type{type}, m_UVs(type == Type::AIR ? nullptr : std::make_unique<std::array<float, Face::COUNT * s_UVsPerFace>>())
{
    genUVs();
}

Block::Block(const Block& other)
    : m_type(other.m_type),
      m_UVs(other.m_UVs ? std::make_unique<std::array<float, Face::COUNT * s_UVsPerFace>>(*other.m_UVs) : nullptr)
{
}

Block& Block::operator=(const Block& other)
{
    if (this != &other) 
    {
        m_type = other.m_type;
        if (other.m_UVs)
        {
            m_UVs = std::make_unique<std::array<float, Face::COUNT * s_UVsPerFace>>(*other.m_UVs);
        }
        else
        {
            m_UVs.reset();
        }
    }
    return *this;
}

bool Block::isFaceVisible(Face face, int x, int y, int z, const std::vector<Block>& blocks) const
{
    switch (face)
    {
        case Face::FRONT:
        {  
            int index = x + Chunk::length * ( (z + 1) + Chunk::width * y);
            return (z == Chunk::width - 1) || (blocks[index].getType() == Type::AIR);
        }
        case Face::BACK:
        {
            int index = x + Chunk::length * ( (z - 1) + Chunk::width * y);
            return (z == 0) || (blocks[index].getType() == Type::AIR);
        }
        case Face::LEFT:
        {
            int index = (x - 1) + Chunk::length * (z + Chunk::width * y);
            return (x == 0) || (blocks[index].getType() == Type::AIR);
        }
        case Face::RIGHT:
        {
            int index = (x + 1) + Chunk::length * (z + Chunk::width * y);
            return (x == Chunk::length - 1) || (blocks[index].getType() == Type::AIR);
        }
        case Face::TOP:
        {
            int index = x + Chunk::length * (z + Chunk::width * (y + 1));
            return (y == Chunk::height - 1) || (blocks[index].getType() == Type::AIR);
        }
        case Face::BOTTOM:
        {
            int index = x + Chunk::length * (z + Chunk::width * (y - 1));
            return (y == 0) || (blocks[index].getType() == Type::AIR);
        }
        default:
        {
            return false;
        }
    }
}

Block::Type Block::getType() const
{
    return m_type;
}

void Block::setType(Block::Type type)
{
    if (type == Type::AIR)
    {
        m_UVs.reset();
    } else if (!m_UVs)
    {
        m_UVs = std::make_unique<std::array<float, Face::COUNT * s_UVsPerFace>>();
    }
    
    if (m_type != type)
	{
		m_type = type;
		genUVs();
	}
}

float Block::getUV(Face face, int index) const
{
    if (m_UVs) {
        return (*m_UVs)[face * s_UVsPerFace + index];
    }
    return -1;
}

void Block::genUVs()
{
    if (m_type != Type::AIR) {
        switch (m_type)
        {
            case Type::GRASS:
                genFaceUV(Face::FRONT , 3, 15);
                genFaceUV(Face::BACK  , 3, 15);
                genFaceUV(Face::LEFT  , 3, 15);
                genFaceUV(Face::RIGHT , 3, 15);
                genFaceUV(Face::TOP   , 7, 13);
                genFaceUV(Face::BOTTOM, 2, 15);
                break;
            case Type::WATER:
                genFaceUV(Face::FRONT , 14, 15);
                genFaceUV(Face::BACK  , 14, 15);
                genFaceUV(Face::LEFT  , 14, 15);
                genFaceUV(Face::RIGHT , 14, 15);
                genFaceUV(Face::TOP   , 14, 15);
                genFaceUV(Face::BOTTOM, 14, 15);
                break;
            case Type::LOG:
                genFaceUV(Face::FRONT , 4, 14);
                genFaceUV(Face::BACK  , 4, 14);
                genFaceUV(Face::LEFT  , 4, 14);
                genFaceUV(Face::RIGHT , 4, 14);
                genFaceUV(Face::TOP   , 5, 14);
                genFaceUV(Face::BOTTOM, 5, 14);
                break;
            case Type::LEAVES:
                genFaceUV(Face::FRONT , 4, 7);
                genFaceUV(Face::BACK  , 4, 7);
                genFaceUV(Face::LEFT  , 4, 7);
                genFaceUV(Face::RIGHT , 4, 7);
                genFaceUV(Face::TOP   , 4, 7);
                genFaceUV(Face::BOTTOM, 4, 7);
                break;
            case Type::DIRT:
                genFaceUV(Face::FRONT , 2, 15);
                genFaceUV(Face::BACK  , 2, 15);
                genFaceUV(Face::LEFT  , 2, 15);
                genFaceUV(Face::RIGHT , 2, 15);
                genFaceUV(Face::TOP   , 2, 15);
                genFaceUV(Face::BOTTOM, 2, 15);
                break;
            case Type::STONE:
                genFaceUV(Face::FRONT , 1, 15);
                genFaceUV(Face::BACK  , 1, 15);
                genFaceUV(Face::LEFT  , 1, 15);
                genFaceUV(Face::RIGHT , 1, 15);
                genFaceUV(Face::TOP   , 1, 15);
                genFaceUV(Face::BOTTOM, 1, 15);
                break;
            case Type::COAL:
                genFaceUV(Face::FRONT , 2, 13);
                genFaceUV(Face::BACK  , 2, 13);
                genFaceUV(Face::LEFT  , 2, 13);
                genFaceUV(Face::RIGHT , 2, 13);
                genFaceUV(Face::TOP   , 2, 13);
                genFaceUV(Face::BOTTOM, 2, 13);
                break;
            default:
                break;
        }
    }
}

void Block::genFaceUV(Face face, float bottomLeftX, float bottomLeftY)
{
    if (m_UVs) {
        constexpr float atlasLength = 16.0f, atlasWidth = 16.0f;

        (*m_UVs)[face * s_UVsPerFace + 0] = bottomLeftX / atlasLength;
        (*m_UVs)[face * s_UVsPerFace + 1] = bottomLeftY / atlasWidth;
        (*m_UVs)[face * s_UVsPerFace + 2] = (bottomLeftX + 1) / atlasLength;
        (*m_UVs)[face * s_UVsPerFace + 3] = bottomLeftY / atlasWidth;
        (*m_UVs)[face * s_UVsPerFace + 4] = (bottomLeftX + 1) / atlasLength;
        (*m_UVs)[face * s_UVsPerFace + 5] = (bottomLeftY + 1) / atlasWidth;
        (*m_UVs)[face * s_UVsPerFace + 6] = bottomLeftX / atlasLength;
        (*m_UVs)[face * s_UVsPerFace + 7] = (bottomLeftY + 1) / atlasWidth;
    }
}
