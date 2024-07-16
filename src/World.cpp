#include "include/World.h"
#include <glm/gtc/noise.hpp>

World::World(float renderDistance)
    : m_renderDistance{renderDistance}
{
    generateHeightMap();
    auto [playerChunkX, playerChunkZ] = getCurrentChunkCoords();

    for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; ++x)
    {
        for (int z = playerChunkZ - renderDistance; z <= playerChunkZ + renderDistance; ++z)
        {
            loadChunk(x, z);
        }
    }
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

void World::generateHeightMap()
{
    m_heightMap.resize(Chunk::Size::length, std::vector<float>(Chunk::Size::width));

    for (int x = 0; x < Chunk::Size::length; ++x)
    {
        for (int z = 0; z < Chunk::Size::width; ++z)
        {
            float height = ((glm::simplex(glm::vec2(x, z) / 16.0f) + 1) / 2 + 1) * 5;
            m_heightMap[x][z] = height;
        }
    }
}

std::pair<int, int> World::getCurrentChunkCoords() const
{
    int chunkXPos = static_cast<int>(m_playerPosition.x) / Chunk::Size::length;
    int chunkZPos = static_cast<int>(m_playerPosition.z) / Chunk::Size::width;
    return {chunkXPos, chunkZPos};
}

void World::loadChunk(int xPos, int zPos)
{
    m_chunks.emplace(std::piecewise_construct,
                     std::forward_as_tuple(xPos, zPos),
                     std::forward_as_tuple(xPos * Chunk::Size::length, zPos * Chunk::Size::width, m_heightMap));
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
}
