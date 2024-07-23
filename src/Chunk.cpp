#include "include/Chunk.h"
#include <random>

Chunk::Chunk(int worldPositionX, int worldPositionZ, const std::vector<std::vector<float>>& heightMap)
    : m_VAO{}, m_VBO{}, m_EBO{}, m_heightMap{heightMap}, m_meshGenerated{false}, m_worldPositionX{worldPositionX}, m_worldPositionZ{worldPositionZ}
{
    m_blocks.resize(Size::length * Size::width * Size::height, Block{});
}

void Chunk::generateBlocks()
{
    for (int x = 0; x < Size::length; ++x)
    {
        for (int z = 0; z < Size::width; ++z)
        {
            int height = (static_cast<int>(m_heightMap[x][z])) < (Size::height) ? (static_cast<int>(m_heightMap[x][z])) : (Size::height);

            if (height > 5 && (rand() % 100) < 10)
                placeTree(x, z, height);

            for (int y = 0; y < Size::height; ++y)
            {
                float blockPosX = m_worldPositionX + x;
                float blockPosY = y;
                float blockPosZ = m_worldPositionZ + z;

                int index = x + Size::length * (z + Size::width * y);

                if (y < s_waterLevel)
                {
                    m_blocks[index] = Block(Block::Type::WATER, blockPosX, blockPosY, blockPosZ);
                }
                else if (y < height - 1)
                {
                    m_blocks[index] = Block(Block::Type::DIRT, blockPosX, blockPosY, blockPosZ);
                }
                else if (y < height)
                {
                    m_blocks[index] = Block(Block::Type::GRASS, blockPosX, blockPosY, blockPosZ);
                }
            }
        }
    }
}

void Chunk::render() const
{
    m_VAO.bind();

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDrawElements(GL_TRIANGLES, m_EBO.getCount(), GL_UNSIGNED_INT, 0);
    m_VAO.unbind();
}

void Chunk::generateMesh()
{
    if (m_meshGenerated) return;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int indexOffset = 0;

    for (int x = 0; x < Size::length; ++x)
    {
        for (int z = 0; z < Size::width; ++z)
        {
            int height = static_cast<int>(m_heightMap[x][z]);
            for (int y = 0; y < Size::height; ++y)
            {
                int index = x + Size::length * (z + Size::width * y);

                if (m_blocks[index].getType() == Block::Type::AIR)
                    continue;

                for (Block::Face face = Block::Face::FRONT; face < Block::Face::COUNT; face = (Block::Face)((int)face + 1))
                {
                    // Check neighboring blocks for visibility
                    if (x == 0 && face == Block::Face::LEFT && m_neighbors[0] && m_neighbors[0]->getBlockAt(Size::length - 1, z, y).getType() != Block::Type::AIR)
                        continue;
                    if (x == Size::length - 1 && face == Block::Face::RIGHT && m_neighbors[1] && m_neighbors[1]->getBlockAt(0, z, y).getType() != Block::Type::AIR)
                        continue;
                    if (z == 0 && face == Block::Face::BACK && m_neighbors[2] && m_neighbors[2]->getBlockAt(x, Size::width - 1, y).getType() != Block::Type::AIR)
                        continue;
                    if (z == Size::width - 1 && face == Block::Face::FRONT && m_neighbors[3] && m_neighbors[3]->getBlockAt(x, 0, y).getType() != Block::Type::AIR)
                        continue;

                    if (m_blocks[index].isFaceVisible(face, x, y, z, m_blocks))
                    {
                        const std::vector<float>& faceVertices = m_blocks[index].getFaceVertices(face);
                        std::vector<unsigned int> faceIndices = {
                            0, 1, 2,
                            2, 3, 0
                        };

                        for (int i = 0; i < faceVertices.size(); i += 5)
                        {
                            vertices.push_back(faceVertices[i] + x + m_worldPositionX);
                            vertices.push_back(faceVertices[i + 1] + y);
                            vertices.push_back(faceVertices[i + 2] + z + m_worldPositionZ);
                            vertices.push_back(faceVertices[i + 3]);
                            vertices.push_back(faceVertices[i + 4]);
                        }

                        for (int i = 0; i < faceIndices.size(); ++i)
                        {
                            indices.push_back(faceIndices[i] + indexOffset);
                        }

                        indexOffset += 4;
                    }
                }
            }
        }
    }

    m_VAO.bind();
    m_VBO.setVertices(vertices.data(), vertices.size() * sizeof(float));

    VertexBufferLayout meshLayout;
    meshLayout.addLayout(GL_FLOAT, 3, GL_FALSE);
    meshLayout.addLayout(GL_FLOAT, 2, GL_FALSE);

    m_VAO.addBuffer(m_VBO, meshLayout);

    m_EBO.bind();
    m_EBO.setElements(indices.data(), indices.size());

    m_VAO.unbind();
    m_VBO.unbind();
    m_EBO.unbind();

    m_meshGenerated = true;
}


void Chunk::placeTree(int x, int z, int y)
{
    constexpr int treeHeight = 5;
    constexpr int treeCrownRadius = 4;
    constexpr int treeCrownHeight = 5;

    if (y + treeHeight + treeCrownHeight / 2 > Size::height 
        || x < treeCrownRadius || x > Size::length - treeCrownRadius || z < treeCrownRadius || z > Size::width - treeCrownRadius)
        return;

    for (int i = 0; i < treeHeight; ++i)
    {
        int index = x + Size::length * (z + Size::width * (y + i));
        if (index < m_blocks.size())
        {
            m_blocks[index] = Block{Block::Type::LOG, m_worldPositionX + x, y + i, m_worldPositionZ + z};
        }
    }

    for (int i = -treeCrownRadius; i <= treeCrownRadius; ++i)
    {
        for (int j = -treeCrownRadius; j <= treeCrownRadius; ++j)
        {
            for (int k = 0; k < treeCrownHeight; ++k)
            {
                int crownY = y + treeHeight - (treeCrownHeight / 2) + k;
                if (crownY >= Size::height) break;

                if (std::abs(i) + std::abs(j) + std::abs(k - treeCrownHeight / 2) <= treeCrownRadius)
                {
                    int index = (x + i) + Size::length * ((z + j) + Size::width * crownY);
                    if (index < m_blocks.size() && (x + i) >= 0 && (x + i) < Size::length && (z + j) >= 0 && (z + j) < Size::width)
                    {
                        m_blocks[index] = Block{Block::Type::LEAVES, m_worldPositionX + i, crownY, m_worldPositionZ + j};
                    }
                }
            }
        }
    }
}

bool Chunk::isMeshGenerated() const
{
    return m_meshGenerated;
}

float Chunk::getWorldPositionX() const
{
    return m_worldPositionX;
}

float Chunk::getWorldPositionZ() const
{
    return m_worldPositionZ;
}

void Chunk::setNeighbors(const std::array<Chunk*, 4>& neighbors)
{
    m_neighbors = neighbors;
}

Block Chunk::getBlockAt(int x, int z, int y) const
{
    int index = x + Size::length * (z + Size::width * y);

    return m_blocks[index];
}

void Chunk::setBlockTypeAt(int x, int z, int y, Block::Type type)
{
    int index = x + Size::length * (z + Size::width * y);

	m_blocks[index].setType(type);
}
