#include "include/World.h"
#include "include/Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

World::World(unsigned int size)
    : m_size{size}
{
    m_chunks.resize(size);

    float worldPositionX = 0, worldPositionZ = 0;
    for (unsigned int z = 0; z < m_chunks.size(); ++z) {
        m_chunks[z].reserve(size); // Reserve space to avoid reallocations
        worldPositionX = 0;
        for (unsigned int x = 0; x < size; ++x) {
            m_chunks[z].emplace_back(worldPositionX, worldPositionZ);
            worldPositionX += Chunk::Size::length;
        }
        worldPositionZ += Chunk::Size::width;
    }
}

void World::render(const Shader& shaderProgram) const
{
    shaderProgram.use();

    for (int z = 0; z < m_chunks.size(); ++z)
    {
        for (int x = 0; x < m_chunks[z].size(); ++x)
        {
            
            glEnable(GL_CULL_FACE);      // Enable face culling
            glFrontFace(GL_CW);          // Set front face to clockwise
            glCullFace(GL_BACK);         // Specify culling of back faces

            float xOffset = -(m_size / 2.0f) * Chunk::Size::length;
            float yOffset = -(m_size / 2.0f) * Chunk::Size::width;

            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 translationVector = glm::vec3(xOffset, 1.0f, yOffset);
            model = glm::translate(model, translationVector);

            shaderProgram.setMat4("model", model);

            m_chunks[z][x].render();
            glDisable(GL_CULL_FACE);
        }
    }
}
