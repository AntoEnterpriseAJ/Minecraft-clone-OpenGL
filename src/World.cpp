#include "include/World.h"
#include <glm/gtc/noise.hpp>
#include <iostream>
#include <future>

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
        chunk->render();
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
    m_chunks[{xPos,zPos}] = new Chunk{xPos * Chunk::Size::length, zPos * Chunk::Size::width};
    m_chunkFutures[{xPos, zPos}] = std::async(std::launch::async, &World::generateChunkBlocks, this, xPos, zPos);
}

void World::generateChunkBlocks(int xPos, int zPos)
{
    m_chunks[{xPos, zPos}]->generateBlocks();
    m_chunks[{xPos, zPos}]->generateMesh();
}

void World::unloadChunk(int xPos, int zPos)
{
    delete m_chunks[{xPos,zPos}];
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

    std::vector<std::pair<int, int>> readyChunks;

    for (auto& [chunkCoords, chunkFuture] : m_chunkFutures)
    {
        const auto status = chunkFuture.wait_for(std::chrono::milliseconds(0));
        if (status == std::future_status::ready)
        {
            chunkFuture.get();
            readyChunks.emplace_back(chunkCoords);
        }
    }

    for (const auto& coords : readyChunks)
    {
        m_chunkFutures.erase(coords);
    }

    updateChunkNeighbors();

    for (const auto& coordinates : readyChunks)
    {
        if (m_chunks.contains(coordinates))
            m_chunks[coordinates]->sendData(); 
    }
}

void World::updateChunkNeighbors()
{
    for (auto& [coordinates, chunk] : m_chunks)
    {
        std::array<Chunk*, 4> neighbors = {
            m_chunks.contains({coordinates.first - 1, coordinates.second}) ?
                m_chunks.at({coordinates.first - 1, coordinates.second}) : nullptr, // LEFT

            m_chunks.contains({coordinates.first + 1, coordinates.second}) ?
                m_chunks.at({coordinates.first + 1, coordinates.second}) : nullptr, // RIGHT

            m_chunks.contains({coordinates.first, coordinates.second - 1}) ?
                m_chunks.at({coordinates.first, coordinates.second - 1}) : nullptr, // BACK

            m_chunks.contains({coordinates.first, coordinates.second + 1}) ?
                m_chunks.at({coordinates.first, coordinates.second + 1}) : nullptr  // FRONT
        };

        chunk->setNeighbors(neighbors);
    }
}

Block World::getBlockAt(int wx, int wz, int wy) const
{
    int chunkX = wx >= 0 ? wx / Chunk::Size::length : (wx - Chunk::Size::length + 1) / Chunk::Size::length;
    int chunkZ = wz >= 0 ? wz / Chunk::Size::width : (wz - Chunk::Size::width + 1) / Chunk::Size::width;

    int localBlockX = wx % Chunk::Size::length;
    if (localBlockX < 0) localBlockX += Chunk::Size::length;

    int localBlockZ = wz % Chunk::Size::width;
    if (localBlockZ < 0) localBlockZ += Chunk::Size::width;

    auto it = m_chunks.find({chunkX, chunkZ});
    if (it == m_chunks.end())
        return Block();

    return it->second->getBlockAt(localBlockX, localBlockZ, wy);
}

void World::setBlockAt(int wx, int wz, int wy, Block::Type type)
{
    int chunkX = wx >= 0 ? wx / Chunk::Size::length : (wx - Chunk::Size::length + 1) / Chunk::Size::length;
    int chunkZ = wz >= 0 ? wz / Chunk::Size::width : (wz - Chunk::Size::width + 1) / Chunk::Size::width;

    int localBlockX = wx % Chunk::Size::length;
    if (localBlockX < 0) localBlockX += Chunk::Size::length;

    int localBlockZ = wz % Chunk::Size::width;
    if (localBlockZ < 0) localBlockZ += Chunk::Size::width;

    auto it = m_chunks.find({chunkX, chunkZ});
    if (it != m_chunks.end())
    {
        it->second->setBlockTypeAt(localBlockX, localBlockZ, wy, type);
    }
}

Chunk* World::getChunkAt(int wx, int wz) const
{
    // Calculate chunk coordinates from world coordinates.
    // Use integer division, considering that negative coordinates should map correctly to chunk indices.
    int chunkX = wx >= 0 ? wx / Chunk::Size::length : (wx - Chunk::Size::length + 1) / Chunk::Size::length;
    int chunkZ = wz >= 0 ? wz / Chunk::Size::width : (wz - Chunk::Size::width + 1) / Chunk::Size::width;

    // Try to find the chunk in the map.
    auto it = m_chunks.find({chunkX, chunkZ});
    if (it == m_chunks.end()) return nullptr;

    // Return the found chunk.
    return it->second;
}
