#include "include/Chunk.h"

Chunk::Chunk(int worldPositionX, int worldPositionZ, const std::vector<std::vector<float>>& heightMap)
    : m_VAO{}, m_VBO{}, m_EBO{}, m_heightMap{heightMap}, m_meshGenerated{false}, m_worldPositionX{worldPositionX}, m_worldPositionZ{worldPositionZ}
{
    m_blocks.resize(Size::length * Size::width * Size::height, Block{});

    for (int x = 0; x < Size::length; ++x)
    {
        for (int z = 0; z < Size::width; ++z)
        {
            int height = (static_cast<int>(heightMap[x][z])) < (Size::height) ? (static_cast<int>(heightMap[x][z])) : (Size::height);
            for (int y = 0; y < Size::height; ++y)
            {
                float blockPosX = worldPositionX + x;
                float blockPosY = y;
                float blockPosZ = worldPositionZ + z;

                int index = x + Size::length * (z + Size::width * y);

                if (y < s_waterLevel)
                {
                    m_blocks[index] = Block(Block::Type::WATER, blockPosX, blockPosY, blockPosZ);
                }
                 
                if (y < height)
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

void Chunk::generateMesh(float worldPositionX, float worldPositionZ)
{
    if (m_meshGenerated) return;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int indexOffset = 0;

    for (int x = 0; x < Size::length; ++x)
    {
        for (int z = 0; z < Size::width; ++z)
        {
            int height = (static_cast<int>(m_heightMap[x][z])) < (Size::height) ? (static_cast<int>(m_heightMap[x][z])) : (Size::height);
            for (int y = 0; y < Size::height; ++y)
            {
                int index = x + Size::length * (z + Size::width * y);

                if (m_blocks[index].getType() == Block::Type::AIR)
                    continue;

                for (Block::Face face = Block::Face::FRONT; face < Block::Face::COUNT; face = (Block::Face)((int)face + 1))
                {
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
                        std::vector<float> faceIndices = {
                            0, 1, 2,
                            2, 3, 0
                        };

                        for (int i = 0; i < faceVertices.size(); i += 5)
                        {
                            vertices.push_back(faceVertices[i] + x + worldPositionX);
                            vertices.push_back(faceVertices[i + 1] + y);
                            vertices.push_back(faceVertices[i + 2] + z + worldPositionZ);
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
