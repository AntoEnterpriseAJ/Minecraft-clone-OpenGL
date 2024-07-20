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

	void render() const;
	Block getBlockAt(int x, int z, int y) const; 
	bool isMeshGenerated() const;
	//float getLocalPositionX() const;
	//float getLocalPositionZ() const;

	void setNeighbors(const std::array<Chunk*, 4>& neighbors);

	Chunk(int localPositionX, int localPositionZ, const std::vector<std::vector<float>>& heightMap);
	void generateMesh(float worldPositionX, float worldPositionZ);
private:

	std::vector<Block> m_blocks;
	std::vector<std::vector<float>> m_heightMap;
	std::array<Chunk*, 4> m_neighbors;

	bool m_meshGenerated;
	float m_worldPositionX;
	float m_worldPositionZ;

	VertexArray m_VAO;
	VertexBuffer m_VBO;
	ElementBuffer m_EBO;
};
