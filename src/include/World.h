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
	std::vector<std::vector<Chunk>> m_chunks;
	unsigned int m_size;
};