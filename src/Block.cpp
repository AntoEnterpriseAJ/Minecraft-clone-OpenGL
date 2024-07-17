#include "include/Block.h"

Block::Block(Type type, int posX, int posY, int posZ)
    : m_type{type}, m_posX{posX}, m_posY{posY}, m_posZ{posZ}
{
    genUVs();

    m_faceVertices[Face::FRONT] = {
        -0.5f, -0.5f,  0.5f,    m_UVs[Face::FRONT][0], m_UVs[Face::FRONT][1], // bottom left
         0.5f, -0.5f,  0.5f,    m_UVs[Face::FRONT][2], m_UVs[Face::FRONT][3], // bottom right
         0.5f,  0.5f,  0.5f,    m_UVs[Face::FRONT][4], m_UVs[Face::FRONT][5], // top right
        -0.5f,  0.5f,  0.5f,    m_UVs[Face::FRONT][6], m_UVs[Face::FRONT][7], // top left
    };

    m_faceVertices[Face::BACK] = {
        -0.5f, -0.5f, -0.5f,    m_UVs[Face::BACK][0], m_UVs[Face::BACK][1], // bottom left
        -0.5f,  0.5f, -0.5f,    m_UVs[Face::BACK][6], m_UVs[Face::BACK][7], // top left
         0.5f,  0.5f, -0.5f,    m_UVs[Face::BACK][4], m_UVs[Face::BACK][5], // top right
         0.5f, -0.5f, -0.5f,    m_UVs[Face::BACK][2], m_UVs[Face::BACK][3], // bottom right
    };

    m_faceVertices[Face::LEFT] = {
        -0.5f, -0.5f, -0.5f,    m_UVs[Face::LEFT][0], m_UVs[Face::LEFT][1], // bottom left
        -0.5f, -0.5f,  0.5f,    m_UVs[Face::LEFT][2], m_UVs[Face::LEFT][3], // bottom right
        -0.5f,  0.5f,  0.5f,    m_UVs[Face::LEFT][4], m_UVs[Face::LEFT][5], // top right
        -0.5f,  0.5f, -0.5f,    m_UVs[Face::LEFT][6], m_UVs[Face::LEFT][7], // top left
    };

    m_faceVertices[Face::RIGHT] = {
         0.5f, -0.5f, -0.5f,    m_UVs[Face::RIGHT][0], m_UVs[Face::RIGHT][1], // bottom left
         0.5f,  0.5f, -0.5f,    m_UVs[Face::RIGHT][6], m_UVs[Face::RIGHT][7], // top left
         0.5f,  0.5f,  0.5f,    m_UVs[Face::RIGHT][4], m_UVs[Face::RIGHT][5], // top right
         0.5f, -0.5f,  0.5f,    m_UVs[Face::RIGHT][2], m_UVs[Face::RIGHT][3], // bottom right
    };

    m_faceVertices[Face::TOP] = {
        -0.5f,  0.5f, -0.5f,    m_UVs[Face::TOP][0], m_UVs[Face::TOP][1], // bottom left
        -0.5f,  0.5f,  0.5f,    m_UVs[Face::TOP][6], m_UVs[Face::TOP][7], // top left
         0.5f,  0.5f,  0.5f,    m_UVs[Face::TOP][4], m_UVs[Face::TOP][5], // top right
         0.5f,  0.5f, -0.5f,    m_UVs[Face::TOP][2], m_UVs[Face::TOP][3], // bottom right
    };

    m_faceVertices[Face::BOTTOM] = {
        -0.5f, -0.5f, -0.5f,    m_UVs[Face::BOTTOM][0], m_UVs[Face::BOTTOM][1], // bottom left
         0.5f, -0.5f, -0.5f,    m_UVs[Face::BOTTOM][2], m_UVs[Face::BOTTOM][3], // bottom right
         0.5f, -0.5f,  0.5f,    m_UVs[Face::BOTTOM][4], m_UVs[Face::BOTTOM][5], // top right
        -0.5f, -0.5f,  0.5f,    m_UVs[Face::BOTTOM][6], m_UVs[Face::BOTTOM][7], // top left
    };
}

bool Block::isFaceVisible(Face face, int x, int y, int z, const std::vector<std::vector<std::vector<Block>>>& blocks) const
{
    int chunkSizeX = blocks.size();
    int chunkSizeZ = blocks[0].size();
    int chunkSizeY = blocks[0][0].size();

    switch (face)
    {
        case Face::FRONT:
        {
            return (z == chunkSizeZ - 1) || (blocks[x][z + 1][y].getType() == Type::AIR);
        }
        case Face::BACK:
        {
            return (z == 0) || (blocks[x][z - 1][y].getType() == Type::AIR);
        }
        case Face::LEFT:
        {
            return (x == 0) || (blocks[x - 1][z][y].getType() == Type::AIR);
        }
        case Face::RIGHT:
        {
            return (x == chunkSizeX - 1) || (blocks[x + 1][z][y].getType() == Type::AIR);
        }
        case Face::TOP:
        {
            return (y == chunkSizeY - 1) || (blocks[x][z][y + 1].getType() == Type::AIR);
        }
        case Face::BOTTOM:
        {
            return (y == 0) || (blocks[x][z][y - 1].getType() == Type::AIR);
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

std::vector<float> Block::getFaceVertices(Face face) const
{
    return m_faceVertices.at(face);
}

void Block::genFaceUV(Face face, float bottomLeftX, float bottomLeftY)
{
    constexpr float atlasLength = 16.0f, atlasWidth = 16.0f;
    m_UVs[face] = {
        bottomLeftX / atlasLength, bottomLeftY / atlasWidth,                // bottom left
        (bottomLeftX + 1) / atlasLength, bottomLeftY / atlasWidth,          // bottom right
        (bottomLeftX + 1) / atlasLength, (bottomLeftY + 1) / atlasWidth,    // top right
        bottomLeftX / atlasLength, (bottomLeftY + 1) / atlasWidth,          // top left
    };
}

void Block::genUVs()
{
    switch (m_type)
    {
        case Type::GRASS:
        {
            genFaceUV(Face::FRONT , 3, 15);
            genFaceUV(Face::BACK  , 3, 15);
            genFaceUV(Face::LEFT  , 3, 15);
            genFaceUV(Face::RIGHT , 3, 15);
            genFaceUV(Face::TOP   , 7, 13);
            genFaceUV(Face::BOTTOM, 2, 15);
            break;
        }
        case Type::AIR:
        {
            genFaceUV(Face::FRONT , 3, 15);
            genFaceUV(Face::BACK  , 3, 15);
            genFaceUV(Face::LEFT  , 3, 15);
            genFaceUV(Face::RIGHT , 3, 15);
            genFaceUV(Face::TOP   , 7, 13);
            genFaceUV(Face::BOTTOM, 2, 15);
            break;
        }
    }
}
