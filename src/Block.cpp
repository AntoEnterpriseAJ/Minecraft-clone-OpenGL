#include "include/Block.h"

Block::Block(Type type, float lengthOffset, float widthOffset, float heightOffset)
    : m_type{type}, m_lengthOffset{lengthOffset}, m_widthOffset{widthOffset}, m_heightOffset{heightOffset}
{
    generateVertices();
}

void Block::addFace(const std::vector<float>& faceVertices)
{
    for (int i = 0; i < faceVertices.size(); ++i)
    {
        if (i % 5 == 0)
            m_vertices.push_back(faceVertices[i] + m_lengthOffset);
        else if (i % 5 == 1)
            m_vertices.push_back(faceVertices[i] + m_widthOffset);
        else if (i % 5 == 2)
            m_vertices.push_back(faceVertices[i] + m_heightOffset);
        else
            m_vertices.push_back(faceVertices[i]);
    }
}

void Block::generateVertices()
{
     std::vector<float> frontFace = {
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
    };
    addFace(frontFace);

    std::vector<float> backFace = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
    };
    addFace(backFace);

    std::vector<float> leftFace = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
    };
    addFace(leftFace);

    std::vector<float> rightFace = {
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
    };
    addFace(rightFace);

    std::vector<float> topFace = {
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f,
    };
    addFace(topFace);

    std::vector<float> bottomFace = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
    };
    addFace(bottomFace);
}

Block::Type Block::getType() const
{
	return m_type;
}

std::vector<float> Block::getVertices() const
{
	return m_vertices;
}
