#include "include/Block.h"

Block::Block(Type type, int posX, int posY, int posZ)
    : m_type{type}, m_posX{posX}, m_posY{posY}, m_posZ{posZ}
{
    genUVs();

    m_faceVertices = {
        -0.5f, -0.5f,  0.5f,    m_UVs[Face::FRONT * s_UVsPerFace + 0], m_UVs[Face::FRONT * s_UVsPerFace + 1],       // bottom left
         0.5f, -0.5f,  0.5f,    m_UVs[Face::FRONT * s_UVsPerFace + 2], m_UVs[Face::FRONT * s_UVsPerFace + 3],       // bottom right
         0.5f,  0.5f,  0.5f,    m_UVs[Face::FRONT * s_UVsPerFace + 4], m_UVs[Face::FRONT * s_UVsPerFace + 5],       // top right
        -0.5f,  0.5f,  0.5f,    m_UVs[Face::FRONT * s_UVsPerFace + 6], m_UVs[Face::FRONT * s_UVsPerFace + 7],       // top left

        -0.5f, -0.5f, -0.5f,    m_UVs[Face::BACK * s_UVsPerFace + 0], m_UVs[Face::BACK * s_UVsPerFace + 1],         // bottom left
        -0.5f,  0.5f, -0.5f,    m_UVs[Face::BACK * s_UVsPerFace + 6], m_UVs[Face::BACK * s_UVsPerFace + 7],         // top left
         0.5f,  0.5f, -0.5f,    m_UVs[Face::BACK * s_UVsPerFace + 4], m_UVs[Face::BACK * s_UVsPerFace + 5],         // top right
         0.5f, -0.5f, -0.5f,    m_UVs[Face::BACK * s_UVsPerFace + 2], m_UVs[Face::BACK * s_UVsPerFace + 3],         // bottom right

        -0.5f, -0.5f, -0.5f,    m_UVs[Face::LEFT * s_UVsPerFace + 0], m_UVs[Face::LEFT * s_UVsPerFace + 1],         // bottom left
        -0.5f, -0.5f,  0.5f,    m_UVs[Face::LEFT * s_UVsPerFace + 2], m_UVs[Face::LEFT * s_UVsPerFace + 3],         // bottom right
        -0.5f,  0.5f,  0.5f,    m_UVs[Face::LEFT * s_UVsPerFace + 4], m_UVs[Face::LEFT * s_UVsPerFace + 5],         // top right
        -0.5f,  0.5f, -0.5f,    m_UVs[Face::LEFT * s_UVsPerFace + 6], m_UVs[Face::LEFT * s_UVsPerFace + 7],         // top left

         0.5f, -0.5f, -0.5f,    m_UVs[Face::RIGHT * s_UVsPerFace + 0], m_UVs[Face::RIGHT * s_UVsPerFace + 1],       // bottom left
         0.5f,  0.5f, -0.5f,    m_UVs[Face::RIGHT * s_UVsPerFace + 6], m_UVs[Face::RIGHT * s_UVsPerFace + 7],       // top left
         0.5f,  0.5f,  0.5f,    m_UVs[Face::RIGHT * s_UVsPerFace + 4], m_UVs[Face::RIGHT * s_UVsPerFace + 5],       // top right
         0.5f, -0.5f,  0.5f,    m_UVs[Face::RIGHT * s_UVsPerFace + 2], m_UVs[Face::RIGHT * s_UVsPerFace + 3],       // bottom right

        -0.5f,  0.5f, -0.5f,    m_UVs[Face::TOP * s_UVsPerFace + 0], m_UVs[Face::TOP * s_UVsPerFace + 1],           // bottom left
        -0.5f,  0.5f,  0.5f,    m_UVs[Face::TOP * s_UVsPerFace + 6], m_UVs[Face::TOP * s_UVsPerFace + 7],           // top left
         0.5f,  0.5f,  0.5f,    m_UVs[Face::TOP * s_UVsPerFace + 4], m_UVs[Face::TOP * s_UVsPerFace + 5],           // top right
         0.5f,  0.5f, -0.5f,    m_UVs[Face::TOP * s_UVsPerFace + 2], m_UVs[Face::TOP * s_UVsPerFace + 3],           // bottom right

        -0.5f, -0.5f, -0.5f,    m_UVs[Face::BOTTOM * s_UVsPerFace + 0], m_UVs[Face::BOTTOM * s_UVsPerFace + 1],      // bottom left
         0.5f, -0.5f, -0.5f,    m_UVs[Face::BOTTOM * s_UVsPerFace + 2], m_UVs[Face::BOTTOM * s_UVsPerFace + 3],      // bottom right
         0.5f, -0.5f,  0.5f,    m_UVs[Face::BOTTOM * s_UVsPerFace + 4], m_UVs[Face::BOTTOM * s_UVsPerFace + 5],      // top right
        -0.5f, -0.5f,  0.5f,    m_UVs[Face::BOTTOM * s_UVsPerFace + 6], m_UVs[Face::BOTTOM * s_UVsPerFace + 7],      // top left
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
    return std::vector<float>(m_faceVertices.begin() + s_verticesPerFace * face, m_faceVertices.begin() + s_verticesPerFace * (face + 1));
}

void Block::genFaceUV(Face face, float bottomLeftX, float bottomLeftY)
{
    constexpr float atlasLength = 16.0f, atlasWidth = 16.0f;

    m_UVs[face * s_UVsPerFace + 0] = bottomLeftX / atlasLength;
    m_UVs[face * s_UVsPerFace + 1] = bottomLeftY / atlasWidth;
    m_UVs[face * s_UVsPerFace + 2] = (bottomLeftX + 1) / atlasLength;
    m_UVs[face * s_UVsPerFace + 3] = bottomLeftY / atlasWidth;
    m_UVs[face * s_UVsPerFace + 4] = (bottomLeftX + 1) / atlasLength;
    m_UVs[face * s_UVsPerFace + 5] = (bottomLeftY + 1) / atlasWidth;
    m_UVs[face * s_UVsPerFace + 6] = bottomLeftX / atlasLength;
    m_UVs[face * s_UVsPerFace + 7] = (bottomLeftY + 1) / atlasWidth;
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
