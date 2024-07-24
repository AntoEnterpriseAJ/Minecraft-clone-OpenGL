#include <iostream>
#include "include/VoxelHandler.h"
#include "GLFW/glfw3.h"


VoxelHandler::VoxelHandler(World& world, glm::vec3 playerPosition, glm::vec3 playerFront)
	: m_world(world), m_playerPosition(playerPosition), m_playerFront(playerFront)
{}

void VoxelHandler::rayCast(glm::vec3 playerPosition, glm::vec3 playerFront)
{
    m_playerPosition = playerPosition;
    m_playerFront = playerFront;

    glm::vec3 startingPosition = m_playerPosition;
    glm::vec3 endPosition = m_playerPosition + m_playerFront * RAYCAST_DISTANCE;
    glm::vec3 currentPosition = startingPosition;

    glm::vec3 step = glm::normalize(endPosition - startingPosition) * 0.1f;

    std::cout << "Raycast started at : " << startingPosition.x << ", " << startingPosition.y << ", " << startingPosition.z << std::endl;

    while (glm::distance(currentPosition, startingPosition) < RAYCAST_DISTANCE)
    {
        int x = static_cast<int>(currentPosition.x);
        int y = static_cast<int>(currentPosition.y);
        int z = static_cast<int>(currentPosition.z);

        Block block = m_world.getBlockAt(x, z, y);
        if (block.getType() != Block::Type::AIR)
        {
            std::cout << "Found block at: " << x << ", " << y << ", " << z << std::endl;
            
			renderSelectedBlockOutline(x, z, y);

			if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				removeSelectedBlock(x, z, y);
			}

            return;
        }

        currentPosition += step;
    }

    std::cout << "Raycast ended without finding non-air block" << std::endl;
}

void VoxelHandler::removeSelectedBlock(int x, int z, int y)
{
	m_world.setBlockAt(x, z, y, Block::Type::AIR);

	int chunkX = x / Chunk::Size::length;
	int chunkZ = z / Chunk::Size::width;

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
}



