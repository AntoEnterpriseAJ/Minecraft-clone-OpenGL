#include "include/World.h"
#include "include/Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

World::World(unsigned int size)
    : m_size{size}
{
    m_chunks.resize(size);
    generateHeightMap();

    float localPositionX = 0, localPositionZ = 0;
    for (unsigned int z = 0; z < m_chunks.size(); ++z) {
        m_chunks[z].reserve(size);
        localPositionX = 0;
        for (unsigned int x = 0; x < size; ++x) {
            m_chunks[z].emplace_back(localPositionX, localPositionZ, m_heightMap);
            localPositionX += Chunk::Size::length;
        }
        localPositionZ += Chunk::Size::width;
    }
}

void World::render(const Shader& shaderProgram) const
{
    shaderProgram.use();

    for (int z = 0; z < m_chunks.size(); ++z)
    {
        for (int x = 0; x < m_chunks[z].size(); ++x)
        {
            
            //glEnable(GL_CULL_FACE);      // Enable face culling
            //glFrontFace(GL_CW);          // Set front face to clockwise
            //glCullFace(GL_BACK);         // Specify culling of back faces

            float xOffset = -(m_size / 2.0f) * Chunk::Size::length;
            float yOffset = -(m_size / 2.0f) * Chunk::Size::width;

            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 translationVector = glm::vec3(xOffset, 1.0f, yOffset);
            model = glm::translate(model, translationVector);

            shaderProgram.setMat4("model", model);

            m_chunks[z][x].render();
            //glDisable(GL_CULL_FACE);
        }
    }
}

void World::generateHeightMap()
{
    int worldLength = m_size * Chunk::Size::length;
    int worldWidth = m_size * Chunk::Size::width;
    m_heightMap.resize(worldLength, std::vector<float>(worldWidth));

    for (int x = 0; x < worldLength; ++x)
    {
        for (int y = 0; y < worldWidth; ++y)
        {
            float value = glm::simplex(glm::vec2{x / 16.0f, y / 16.0f});
            value = (value + 1) / 2 + 1;
            value *= 5;

            m_heightMap[x][y] = value;
        }
    }
}
