#pragma once

#include <vector>
#include "Block.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "ElementBuffer.h"

class Chunk
{
public:
	enum Size
	{
		length = 16,
		width = 16,
		height = 32,
	};

	void render() const;
	float getLocalPositionX() const;
	float getLocalPositionZ() const;

	Chunk(int localPositionX, int localPositionZ, const std::vector<std::vector<float>>& heightMap);
private:
	void generateMesh(float worldPositionX, float worldPositionZ, const std::vector<std::vector<float>>& heightMap);
private:
	std::vector<std::vector<std::vector<Block>>> m_blocks;
	float m_worldPositionX;
	float m_worldPositionZ;
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	ElementBuffer m_EBO;
};
