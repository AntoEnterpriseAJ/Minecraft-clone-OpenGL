#pragma once

#include <vector>
#include <unordered_map>
#include "Chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class World
{
public:
	World(float renderDistance);

	void render(const glm::vec3& playerPosition);
private:
	std::vector<std::vector<float>> generateHeightMap(int chunkX, int chunkZ);
	std::pair<int, int> getCurrentChunkCoords() const;
	void loadChunk(int xPos, int zPos);
	void unloadChunk(int xPos, int zPos);
	void update();

	struct PairHash
	{
		std::size_t operator()(const std::pair<int, int>& pair) const
		{
			return std::hash<int>{}(pair.first) ^ std::hash<int>{}(pair.second);
		}
	};

	std::unordered_map<std::pair<int, int>, Chunk, PairHash> m_chunks;
	glm::vec3 m_playerPosition;
	float m_renderDistance;
};
