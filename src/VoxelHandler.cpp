#include <iostream>
#include "include/VoxelHandler.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

VoxelHandler::VoxelHandler(World& world, glm::vec3 playerPosition, glm::vec3 playerFront)
	: m_world(world), m_playerPosition(playerPosition), m_playerFront(playerFront)
{}

void VoxelHandler::rayCast(glm::vec3 playerPosition, glm::vec3 playerFront)
{
	auto sign	= [](float x){return (x > 0) - (x < 0);};
	auto frac0	= [](float x){return x - std::floor(x);};
	auto frac1	= [](float x){return 1 - (x - std::floor(x));};

	float tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ;
	glm::ivec3 voxel;
	glm::ivec3 voxelNormal = glm::ivec3(0, 0, 0);
	glm::vec3 start = playerPosition;
	glm::vec3 end = playerPosition + playerFront * RAYCAST_DISTANCE;
	int stepDirection = -1;

    int dx = sign(end.x - start.x);
    if (dx != 0) tDeltaX = std::min(dx / (end.x - start.x), 10000000.0f);
		else tDeltaX = 10000000.0f;
    tMaxX = (dx > 0) ? tDeltaX * frac1(start.x) : tDeltaX * frac0(start.x);
    voxel.x = static_cast<int>(start.x);

    int dy = sign(end.y - start.y);
    if (dy != 0) tDeltaY = std::min(dy / (end.y - start.y), 10000000.0f);
		else tDeltaY = 10000000.0f;
    tMaxY = (dy > 0) ? tDeltaY * frac1(start.y) : tDeltaY * frac0(start.y);
    voxel.y = static_cast<int>(start.y);

    int dz = sign(end.z - start.z);
    if (dz != 0) tDeltaZ = std::min(dz / (end.z - start.z), 10000000.0f);
		else tDeltaZ = 10000000.0f;
    tMaxZ = (dz > 0) ? tDeltaZ * frac1(start.z) : tDeltaZ * frac0(start.z);
    voxel.z = static_cast<int>(start.z);

    while (true) 
	{
		int x = static_cast<int>(voxel.x);
		int y = static_cast<int>(voxel.y);
		int z = static_cast<int>(voxel.z);

		if (m_world.getBlockAt(x, z, y).getType() != Block::Type::AIR)
		{
			renderSelectedBlockOutline(voxel.x, voxel.z, voxel.y);
			//std::cout << "found non-air block at: " << x << ", " << y << ", " << z << "\n";

			if (stepDirection == 0) voxelNormal.x = -dx;
			else if (stepDirection == 1) voxelNormal.y = -dy;
			else if (stepDirection == 2) voxelNormal.z = -dz;

			processVoxel(voxel, voxelNormal);

			break;
		}

        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                voxel.x += dx;
                tMaxX += tDeltaX;
				stepDirection = 0;
            } else {
                voxel.z += dz;
                tMaxZ += tDeltaZ;
				stepDirection = 2;
            }
        } else {
            if (tMaxY < tMaxZ) {
                voxel.y += dy;
                tMaxY += tDeltaY;
				stepDirection = 1;
            } else {
                voxel.z += dz;
                tMaxZ += tDeltaZ;
				stepDirection = 2;
            }
        }

        if (tMaxX > 1.0f && tMaxY > 1.0f && tMaxZ > 1.0f) break;
    }
}

void VoxelHandler::processVoxel(glm::ivec3 voxel, glm::ivec3 voxelNormal)
{
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		removeSelectedBlock(voxel.x, voxel.z, voxel.y);
	}
	else if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		addBlock(voxel, voxelNormal);
	}
}

void VoxelHandler::addBlock(glm::ivec3 voxel, glm::ivec3 voxelNormal)
{
	glm::vec3 addPosition = voxel + voxelNormal;

	if (m_world.getBlockAt(addPosition.x, addPosition.z, addPosition.y).getType() == Block::Type::AIR)
	{
		m_world.setBlockAt(addPosition.x, addPosition.z, addPosition.y, Block::Type::LOG);

		m_world.getChunkAt(voxel.x, voxel.z)->m_meshGenerated = false;
		m_world.getChunkAt(voxel.x, voxel.z)->generateMesh();
	}
}

void VoxelHandler::removeSelectedBlock(int x, int z, int y)
{
	m_world.setBlockAt(x, z, y, Block::Type::AIR);

	m_world.getChunkAt(x, z)->m_meshGenerated = false;
	m_world.getChunkAt(x, z)->generateMesh();
}

void VoxelHandler::renderSelectedBlockOutline(int x, int z, int y)
{
	glm::vec3 worldPosition = glm::vec3(x, y, z);

	std::vector<glm::vec3> vertices = {
		// Front face
		worldPosition + glm::vec3(-0.51f, -0.51f, -0.51f),
		worldPosition + glm::vec3(0.51f, -0.51f, -0.51f),
		worldPosition + glm::vec3(0.51f, 0.51f, -0.51f),
		worldPosition + glm::vec3(-0.51f, 0.51f, -0.51f),

		// Back face
		worldPosition + glm::vec3(-0.51f, -0.51f, 0.51f),
		worldPosition + glm::vec3(0.51f, -0.51f, 0.51f),
		worldPosition + glm::vec3(0.51f, 0.51f, 0.51f),
		worldPosition + glm::vec3(-0.51f, 0.51f, 0.51f)
	};

	std::vector<unsigned int> indices = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5.0f);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0f);
}



