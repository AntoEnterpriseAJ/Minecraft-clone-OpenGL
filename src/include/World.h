#pragma once
#include <vector>
#include "Chunk.h"
#include "Shader.h"

class World
{
public:
	void render(const Shader& shaderProgram) const;

	World(unsigned int size);
private:
	std::vector<std::vector<Chunk>> m_chunks;
	unsigned int m_size;
};