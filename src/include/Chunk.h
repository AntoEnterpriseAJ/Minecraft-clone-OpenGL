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
	float getLocalPositionX() const;
	float getLocalPositionZ() const;

	Chunk(int localPositionX = 0, int localPositionZ = 0/*, const std::vector<std::vector<float>>& heightMap*/);
private:
	void generateMesh(float worldPositionX, float worldPositionZ);
private:
	std::vector<std::vector<std::vector<Block>>> m_blocks;
	float m_worldPositionX;
	float m_worldPositionZ;
	VertexArray m_VAO;
	VertexBuffer m_VBO;
};
