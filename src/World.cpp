#include "include/World.h"
#include <glm/gtc/noise.hpp>
#include <thread>

World::World(float renderDistance)
    : m_renderDistance{renderDistance}
{
    auto [playerChunkX, playerChunkZ] = getCurrentChunkCoords();

    for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; ++x)
    {
        for (int z = playerChunkZ - renderDistance; z <= playerChunkZ + renderDistance; ++z)
        {
            loadChunk(x, z);
        }
    }

    updateChunkNeighbors();
}

void World::render(const glm::vec3& playerPosition)
{
    m_playerPosition = playerPosition;
    update();

    for (const auto& [coordinates, chunk] : m_chunks)
    {
        chunk.render();
    }
}

std::vector<std::vector<float>> World::generateHeightMap(int chunkX, int chunkZ)
{
    std::vector<std::vector<float>> heightMap;
    heightMap.resize(Chunk::Size::length, std::vector<float>(Chunk::Size::width));

    for (int x = 0; x < Chunk::Size::length; ++x)
    {
        for (int z = 0; z < Chunk::Size::width; ++z)
        {
            float chunkWorldX = x + chunkX * Chunk::Size::length;
            float chunkWorldZ = z + chunkZ * Chunk::Size::width;

            float height = glm::simplex(glm::vec2(chunkWorldX / 64.0f, chunkWorldZ / 64.0f));
            height = (height + 1.0f) / 2.0f * 10.0f;

            heightMap[x][z] = std::abs(height) == 0 ? std::abs(height) + 1 : std::abs(height);
        }
    }

    return heightMap;
}

std::pair<int, int> World::getCurrentChunkCoords() const
{
    int chunkXPos = static_cast<int>(m_playerPosition.x) / Chunk::Size::length;
    int chunkZPos = static_cast<int>(m_playerPosition.z) / Chunk::Size::width;
    return {chunkXPos, chunkZPos};
}

void World::loadChunk(int xPos, int zPos)
{
    const auto& heightMap = generateHeightMap(xPos,zPos);

    m_chunks.emplace(std::piecewise_construct,
                     std::forward_as_tuple(xPos, zPos),
                     std::forward_as_tuple(xPos * Chunk::Size::length, zPos * Chunk::Size::width, heightMap));
}

void World::unloadChunk(int xPos, int zPos)
{
    m_chunks.erase({xPos, zPos});
}

void World::update()
{
    auto [playerChunkX, playerChunkZ] = getCurrentChunkCoords();

    std::vector<std::pair<int, int>> chunksToUnload;
    for (const auto& [coordinates, chunk] : m_chunks)
    {
        int chunkX = coordinates.first, chunkZ = coordinates.second;
        if (std::abs(chunkX - playerChunkX) > m_renderDistance || std::abs(chunkZ - playerChunkZ) > m_renderDistance)
        {
            chunksToUnload.push_back(coordinates);
        }
    }

    for (const auto& coordinates : chunksToUnload)
    {
        unloadChunk(coordinates.first, coordinates.second);
    }

    for (int x = playerChunkX - m_renderDistance; x <= playerChunkX + m_renderDistance; ++x)
    {
        for (int z = playerChunkZ - m_renderDistance; z <= playerChunkZ + m_renderDistance; ++z)
        {
            if (!m_chunks.contains({x, z}))
            {
                loadChunk(x, z);
            }
        }
    }

    updateChunkNeighbors();

    for (auto& [coordinates, chunk] : m_chunks)
    {
        chunk.generateMesh(coordinates.first * Chunk::Size::length, coordinates.second * Chunk::Size::width);
    }
}

void World::updateChunkNeighbors()
{
    for (auto& [coordinates, chunk] : m_chunks)
    {
        std::array<Chunk*, 4> neighbors = {
            m_chunks.contains({coordinates.first - 1, coordinates.second}) ?
                &m_chunks.at({coordinates.first - 1, coordinates.second}) : nullptr, // LEFT

            m_chunks.contains({coordinates.first + 1, coordinates.second}) ?
                &m_chunks.at({coordinates.first + 1, coordinates.second}) : nullptr, // RIGHT

            m_chunks.contains({coordinates.first, coordinates.second - 1}) ?
                &m_chunks.at({coordinates.first, coordinates.second - 1}) : nullptr, // BACK

            m_chunks.contains({coordinates.first, coordinates.second + 1}) ?
                &m_chunks.at({coordinates.first, coordinates.second + 1}) : nullptr  // FRONT
        };

        chunk.setNeighbors(neighbors);
    }
}