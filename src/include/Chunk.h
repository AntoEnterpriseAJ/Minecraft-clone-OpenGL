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
		height = 256,
	};

	Chunk(int worldPositionX, int worldPositionZ);

	void render() const;
	Block getBlockAt(int x, int z, int y) const;
	void setBlockTypeAt(int x, int z, int y, Block::Type type);
	bool isMeshGenerated() const;
	float getWorldPositionX() const;
	float getWorldPositionZ() const;

	void generateBlocks();
	void setNeighbors(const std::array<Chunk*, 4>& neighbors);
	void generateMesh();
	void placeTree(int x, int z, int y);

	bool m_meshGenerated;

private:
	int getHeightAt(int x, int z) const;
	static constexpr int s_waterLevel = 2;

	std::vector<Block> m_blocks;
	std::array<Chunk*, 4> m_neighbors;

	int m_worldPositionX;
	int m_worldPositionZ;

	VertexArray m_VAO;
	VertexBuffer m_VBO;
	ElementBuffer m_EBO;
};
