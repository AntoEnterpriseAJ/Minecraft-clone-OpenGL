#pragma once

#include <vector>
#include "Block.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class Chunk
{
public:
	enum Size
	{
		length = 16,
		width = 16,
		height = 16,
	};

	void render() const;
	float getWorldPositionX() const;
	float getWorldPositionZ() const;

	Chunk(float worldPositionX = 0, float worldPositionZ = 0);
private:
	void generateMesh(float worldPositionX, float worldPositionZ);
private:
	std::vector<std::vector<std::vector<Block>>> m_blocks;
	float m_worldPositionX;
	float m_worldPositionZ;
	VertexArray m_VAO;
	VertexBuffer m_VBO;
};
