#pragma once
#include <vector>
#include "Chunk.h"
#include "Shader.h"

class World
{
public:
	World(unsigned int size);

	void render(const Shader& shaderProgram) const;
private:
	void generateHeightMap();

	std::vector<std::vector<Chunk>> m_chunks;
	std::vector<std::vector<float>> m_heightMap;
	unsigned int m_size;
};