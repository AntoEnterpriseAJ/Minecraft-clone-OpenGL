#include "include/Block.h"

Block::Block(Type type, int posX, int posY, int posZ)
    : m_type{type}, m_posX{posX}, m_posY{posY}, m_posZ{posZ}
{
    generateVertices();
}
void Block::addFace(const std::vector<float>& faceVertices, const std::vector<unsigned int>& baseIndices)
{
    unsigned int indexOffset = m_vertices.size() / 5;

    for (int i = 0; i < faceVertices.size(); i += 5)
    {
        m_vertices.push_back(faceVertices[i] + m_posX);
        m_vertices.push_back(faceVertices[i + 1] + m_posY);
        m_vertices.push_back(faceVertices[i + 2] + m_posZ);
        m_vertices.push_back(faceVertices[i + 3]);
        m_vertices.push_back(faceVertices[i + 4]);
    }

    for (auto index : baseIndices)
    {
        m_indices.push_back(index + indexOffset);
    }
}

void Block::generateVertices()
{
    std::vector<unsigned int> baseIndices = {
        0, 1, 2,
        2, 3, 0
    };

    std::vector<float> frontFace = {
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, // bottom left
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // bottom right
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, // top left
    };
    addFace(frontFace, baseIndices);

    std::vector<float> backFace = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // bottom right
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // top left
    };
    addFace(backFace, baseIndices);

    std::vector<float> leftFace = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // bottom right
        -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // top left
    };
    addFace(leftFace, baseIndices);

    std::vector<float> rightFace = {
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // bottom right
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // top left
    };
    addFace(rightFace, baseIndices);

    std::vector<float> topFace = {
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, // bottom right
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // top right
         0.5f,  0.5f, -0.5f,    1.0f, 0.0f, // top left
    };
    addFace(topFace, baseIndices);

    std::vector<float> bottomFace = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // bottom right
         0.5f, -0.5f,  0.5f,    1.0f, 1.0f, // top right
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f, // top left
    };
    addFace(bottomFace, baseIndices);
}

Block::Type Block::getType() const
{
    return m_type;
}

std::vector<float> Block::getVertices() const
{
    return m_vertices;
}

std::vector<unsigned int> Block::getIndices() const
{
    return m_indices;
}
