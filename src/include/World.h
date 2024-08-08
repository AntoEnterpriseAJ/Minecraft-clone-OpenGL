#pragma once

#include <vector>
#include <unordered_map>
#include "Chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <future>

class World
{
public:
	World(float renderDistance);

	void render(const glm::vec3& playerPosition);
	Block getBlockAt(int wx, int wz, int wy) const;
	void setBlockAt(int wx, int wz, int wy, Block::Type type);
	Chunk* getChunkAt(int wx, int wz) const;

private:
	std::pair<int, int> getCurrentChunkCoords() const;

	void loadChunk(int xPos, int zPos);
	void generateChunkBlocks(int xPos, int zPos);
	void unloadChunk(int xPos, int zPos);
	void update();
	void updateChunkNeighbors();

	struct PairHash
	{
		std::size_t operator()(const std::pair<int, int>& pair) const
		{
			return std::hash<int>{}(pair.first) ^ std::hash<int>{}(pair.second);
		}
	};
private:
	std::unordered_map<std::pair<int, int>, Chunk*, PairHash> m_chunks;
	std::unordered_map<std::pair<int, int>, std::future<void>, PairHash> m_chunkFutures;
	glm::vec3 m_playerPosition;
	float m_renderDistance;
};
