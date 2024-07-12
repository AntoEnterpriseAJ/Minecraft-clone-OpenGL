#pragma once

#include <vector>
#include "Block.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class Chunk
{
public:
	void render() const;

	Chunk();
private:
	void generateMesh();
private:
	enum Size
	{
		length = 16,
		width = 16,
		height = 64,
	};
private:
	std::vector<std::vector<std::vector<Block>>> m_blocks;
	VertexArray m_VAO;
	VertexBuffer m_VBO;
};
