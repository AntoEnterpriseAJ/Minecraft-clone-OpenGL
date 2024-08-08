#pragma once
#include "World.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class VoxelHandler
{
public:
	VoxelHandler(World& world, glm::vec3 playerPosition, glm::vec3 playerFront);

	void rayCast(glm::vec3 playerPosition, glm::vec3 playerFront);

private:
	void processVoxel(glm::ivec3 voxel, glm::ivec3 voxelNormal);
	void renderSelectedBlockOutline(int x, int z, int y);
	void removeSelectedBlock(int x, int z, int y);
	void addBlock(glm::ivec3 voxel, glm::ivec3 voxelNormal);
private:
	World& m_world;
	glm::vec3 m_playerPosition;
	glm::vec3 m_playerFront;
private:
	static constexpr float RAYCAST_DISTANCE = 6;
};