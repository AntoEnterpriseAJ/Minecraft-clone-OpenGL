#include "include/Chunk.h"
#include <random>
#include <glm/gtc/noise.hpp>

Chunk::Chunk(int worldPositionX, int worldPositionZ)
    : m_VAO{}, m_VBO{}, m_EBO{}, m_meshGenerated{false}, m_worldPositionX{worldPositionX}, m_worldPositionZ{worldPositionZ}
{
    m_blocks.resize(Size::length * Size::width * Size::height, Block{});
}

void Chunk::generateBlocks()
{
    for (int x = 0; x < Size::length; ++x)
    {
        for (int z = 0; z < Size::width; ++z)
        {
            int height = (getHeightAt(x, z)) < (Size::height) ? (getHeightAt(x, z)) : (Size::height);

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
                    m_blocks[index] = Block(Block::Type::WATER);
                }
                else if (y < height - 4)
                {
                    if (rand() % 100 < 10)
						m_blocks[index] = Block(Block::Type::COAL);
					else
						m_blocks[index] = Block(Block::Type::STONE);
                }
                else if (y < height - 1)
                {
                    m_blocks[index] = Block(Block::Type::DIRT);
                }
                else if (y < height)
                {
                    m_blocks[index] = Block(Block::Type::GRASS);
                }
            }
        }
    }
}

void Chunk::render() const
{
    m_VAO.bind();

    glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
            int height = getHeightAt(x, z);
            for (int y = 0; y < Size::height; ++y)
            {
                int index = x + Size::length * (z + Size::width * y);

                const Block& block = m_blocks[index];

                if (block.getType() == Block::Type::AIR)
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

                    if (block.isFaceVisible(face, x, y, z, m_blocks))
                    {
                        constexpr int verticesPerFace = 20;

                        const std::array<float, verticesPerFace * Block::Face::COUNT> blockVertices = {
                            -0.5f, -0.5f,  0.5f, block.getUV(Block::Face::FRONT, 0), block.getUV(Block::Face::FRONT, 1),      // bottom left
                             0.5f, -0.5f,  0.5f, block.getUV(Block::Face::FRONT, 2), block.getUV(Block::Face::FRONT, 3),      // bottom right
                             0.5f,  0.5f,  0.5f, block.getUV(Block::Face::FRONT, 4), block.getUV(Block::Face::FRONT, 5),      // top right
                            -0.5f,  0.5f,  0.5f, block.getUV(Block::Face::FRONT, 6), block.getUV(Block::Face::FRONT, 7),      // top left
                                                                                                                              
                            -0.5f, -0.5f, -0.5f, block.getUV(Block::Face::BACK, 0), block.getUV(Block::Face::BACK, 1),        // bottom left
                            -0.5f,  0.5f, -0.5f, block.getUV(Block::Face::BACK, 6), block.getUV(Block::Face::BACK, 7),        // top left
                             0.5f,  0.5f, -0.5f, block.getUV(Block::Face::BACK, 4), block.getUV(Block::Face::BACK, 5),        // top right
                             0.5f, -0.5f, -0.5f, block.getUV(Block::Face::BACK, 2), block.getUV(Block::Face::BACK, 3),        // bottom right
                                                                                                                              
                            -0.5f, -0.5f, -0.5f, block.getUV(Block::Face::LEFT, 0), block.getUV(Block::Face::LEFT, 1),        // bottom left
                            -0.5f, -0.5f,  0.5f, block.getUV(Block::Face::LEFT, 2), block.getUV(Block::Face::LEFT, 3),        // bottom right
                            -0.5f,  0.5f,  0.5f, block.getUV(Block::Face::LEFT, 4), block.getUV(Block::Face::LEFT, 5),        // top right
                            -0.5f,  0.5f, -0.5f, block.getUV(Block::Face::LEFT, 6), block.getUV(Block::Face::LEFT, 7),        // top left
                                                                                                                              
                             0.5f, -0.5f, -0.5f, block.getUV(Block::Face::RIGHT, 0), block.getUV(Block::Face::RIGHT, 1),      // bottom left
                             0.5f,  0.5f, -0.5f, block.getUV(Block::Face::RIGHT, 6), block.getUV(Block::Face::RIGHT, 7),      // top left
                             0.5f,  0.5f,  0.5f, block.getUV(Block::Face::RIGHT, 4), block.getUV(Block::Face::RIGHT, 5),      // top right
                             0.5f, -0.5f,  0.5f, block.getUV(Block::Face::RIGHT, 2), block.getUV(Block::Face::RIGHT, 3),      // bottom right
                                                                                                                              
                            -0.5f,  0.5f, -0.5f, block.getUV(Block::Face::TOP, 0), block.getUV(Block::Face::TOP, 1),          // bottom left
                            -0.5f,  0.5f,  0.5f, block.getUV(Block::Face::TOP, 6), block.getUV(Block::Face::TOP, 7),          // top left
                             0.5f,  0.5f,  0.5f, block.getUV(Block::Face::TOP, 4), block.getUV(Block::Face::TOP, 5),          // top right
                             0.5f,  0.5f, -0.5f, block.getUV(Block::Face::TOP, 2), block.getUV(Block::Face::TOP, 3),          // bottom right
                                                                                                                              
                            -0.5f, -0.5f, -0.5f, block.getUV(Block::Face::BOTTOM, 0), block.getUV(Block::Face::BOTTOM, 1),    // bottom left
                             0.5f, -0.5f, -0.5f, block.getUV(Block::Face::BOTTOM, 2), block.getUV(Block::Face::BOTTOM, 3),    // bottom right
                             0.5f, -0.5f,  0.5f, block.getUV(Block::Face::BOTTOM, 4), block.getUV(Block::Face::BOTTOM, 5),    // top right
                            -0.5f, -0.5f,  0.5f, block.getUV(Block::Face::BOTTOM, 6), block.getUV(Block::Face::BOTTOM, 7),    // top left
                        };

                        const std::vector<float> faceVertices(blockVertices.begin() + verticesPerFace * face, blockVertices.begin() + verticesPerFace * (face + 1));

                        std::array<unsigned int, 6> faceIndices = {
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
                            vertices.push_back(Block::s_faceNormals[face * 3 + 0]);
                            vertices.push_back(Block::s_faceNormals[face * 3 + 1]);
                            vertices.push_back(Block::s_faceNormals[face * 3 + 2]);
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
    meshLayout.addLayout(GL_FLOAT, 3, GL_FALSE);

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
            m_blocks[index] = Block{Block::Type::LOG};
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
                        m_blocks[index] = Block{Block::Type::LEAVES};
                    }
                }
            }
        }
    }
}

int Chunk::getHeightAt(int x, int z) const
{
    float chunkWorldX = m_worldPositionX + x;
    float chunkWorldZ = m_worldPositionZ + z;

    float height = glm::simplex(glm::vec2(chunkWorldX / 64.0f, chunkWorldZ / 64.0f));
    height = (height + 1.0f) / 2.0f * 20.0f;

    return height;
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
