#include "include/Chunk.h"

#include <algorithm>

void Chunk::render() const
{
    m_VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, Size::length * Size::width * Size::height * 6 * 2 * 3);
    m_VAO.unbind();
}

Chunk::Chunk(int localPositionX, int localPositionZ/*, const std::vector<std::vector<float>>& heightMap*/)
    : m_VAO{}, m_VBO{}
{
    m_blocks.resize(Size::length, std::vector<std::vector<Block>>(Size::width, std::vector<Block>(Size::height)));
    for (int x = 0; x < Size::length; ++x)
    {
        for (int z = 0; z < Size::width; ++z)
        {
            int height = Size::height;/*(static_cast<int>(heightMap[x][z])) < (Size::height) ? (static_cast<int>(heightMap[x][z])) : (Size::height);*/
            for (int y = 0; y < height; ++y)
            {
                float blockPosX = localPositionX + x;
                float blockPosY = y;
                float blockPosZ = localPositionZ + z;
                m_blocks[x][z][y] = Block(Block::Type::GRASS, blockPosX, blockPosY, blockPosZ);
            }
        }
    }

    generateMesh(localPositionX, localPositionZ);
}

float Chunk::getLocalPositionX() const
{
    return m_worldPositionX;
}

float Chunk::getLocalPositionZ() const
{
    return m_worldPositionZ;
}

void Chunk::generateMesh(float worldPositionX, float worldPositionZ)
{
    std::vector<float> vertices;

    for (int x = 0; x < Size::length; ++x)
    {
        for (int y = 0; y < Size::height; ++y)
        {
            for (int z = 0; z < Size::width; ++z)
            {
                std::vector<float> currentVertices = m_blocks[x][y][z].getVertices();
                vertices.insert(vertices.end(), currentVertices.begin(), currentVertices.end());
            }
        }
    }

    m_VAO.bind();
    m_VBO.setVertices(&vertices[0], vertices.size() * sizeof(float));

    VertexBufferLayout meshLayout;
    meshLayout.addLayout(GL_FLOAT, 3, GL_FALSE);
    meshLayout.addLayout(GL_FLOAT, 2, GL_FALSE);

    m_VAO.addBuffer(m_VBO, meshLayout);
    m_VAO.unbind();
    m_VBO.unbind();
}
