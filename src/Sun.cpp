#include "include/Sun.h"
#include "include/VertexBufferLayout.h"

Sun::Sun()
	: m_VAO{}, m_VBO{}, m_EBO{}, m_worldPosition{}
{
	m_VAO.bind();

	m_VBO.bind();
	m_VBO.setVertices(s_sunVertices, sizeof(s_sunVertices));

	VertexBufferLayout layout;
	layout.addLayout(GL_FLOAT, 3, GL_FALSE);

	m_VAO.addBuffer(m_VBO, layout);

	m_EBO.bind();
	m_EBO.setElements(s_sunIndices, sizeof(s_sunIndices) / sizeof(s_sunIndices[0]));
}

void Sun::updatePosition(glm::vec3 cameraPosition, float deltaTime)
{
	constexpr float distanceFromCamera = 400.0f;
	static float angle = 0.0f;
	angle += deltaTime * 0.1f;

	m_worldPosition = cameraPosition + glm::vec3(
		distanceFromCamera * cos(angle),
		distanceFromCamera * sin(angle),
		distanceFromCamera * sin(angle)
	);
}

void Sun::render(Shader* shader)
{
	shader->use();

	m_VAO.bind();

	glDrawElements(GL_TRIANGLES, sizeof(s_sunIndices) / sizeof(s_sunIndices[0]), GL_UNSIGNED_INT, 0);
}

glm::vec3 Sun::getPosition() const
{
	return m_worldPosition;
}
