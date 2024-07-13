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
            // Calculate the model matrix for each chunk
            glm::mat4 model = glm::mat4(1.0f);

            float xOffset = -(m_size / 2.0f) * Chunk::Size::length;
            float yOffset = -(m_size / 2.0f) * Chunk::Size::width;
            glm::vec3 translationVector = glm::vec3(xOffset, 1.0f, yOffset);
            model = glm::translate(model, translationVector);

            int modelLocation = glGetUniformLocation(shaderProgram.getID(), "model");
            if (modelLocation == -1) {
                std::cerr << "Warning: Unable to find uniform location for 'model'" << std::endl;
            }
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            m_chunks[z][x].render();
        }
    }
}
