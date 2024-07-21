#pragma once

#include <vector>
#include <array>
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

	Chunk(int worldPositionX, int worldPositionZ, const std::vector<std::vector<float>>& heightMap);

	void render() const;
	Block getBlockAt(int x, int z, int y) const; 
	bool isMeshGenerated() const;
	float getWorldPositionX() const;
	float getWorldPositionZ() const;

	void setNeighbors(const std::array<Chunk*, 4>& neighbors);
	void generateMesh(float worldPositionX, float worldPositionZ);

private:
	static constexpr int s_waterLevel = 2;

	std::vector<Block> m_blocks;
	std::vector<std::vector<float>> m_heightMap;
	std::array<Chunk*, 4> m_neighbors;

	bool m_meshGenerated;
	int m_worldPositionX;
	int m_worldPositionZ;

	VertexArray m_VAO;
	VertexBuffer m_VBO;
	ElementBuffer m_EBO;
};
