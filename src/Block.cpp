#include "include/Block.h"

Block::Block(Type type, int posX, int posY, int posZ)
    : m_type{type}, m_posX{posX}, m_posY{posY}, m_posZ{posZ}
{
    m_faceVertices[Face::FRONT] = {
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, // bottom left
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // bottom right
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, // top left
    };

    m_faceVertices[Face::BACK] = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // bottom right
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // top left
    };

    m_faceVertices[Face::LEFT] = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // bottom right
        -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // top left
    };

    m_faceVertices[Face::RIGHT] = {
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // bottom right
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // top left
	};

    m_faceVertices[Face::TOP] = {
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, // bottom left
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f, // bottom right
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
		 0.5f,  0.5f, -0.5f,    1.0f, 0.0f, // top left
	};

    m_faceVertices[Face::BOTTOM] = {
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
		 0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // bottom right
		 0.5f, -0.5f,  0.5f,    1.0f, 1.0f, // top right
		-0.5f, -0.5f,  0.5f,    0.0f, 1.0f, // top left
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
