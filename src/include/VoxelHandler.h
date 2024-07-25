#pragma once
#include "World.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class VoxelHandler
{
public:
	VoxelHandler(World& world, glm::vec3 playerPosition, glm::vec3 playerFront);

	void rayCast(glm::vec3 playerPosition, glm::vec3 playerFront);
	void processVoxel(glm::ivec3 voxel);
private:
	void renderSelectedBlockOutline(int x, int z, int y);
	void removeSelectedBlock(int x, int z, int y);

	static constexpr float RAYCAST_DISTANCE = 6;

	World& m_world;
	glm::vec3 m_playerPosition;
	glm::vec3 m_playerFront;
};