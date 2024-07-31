#include "include/Crosshair.h"
#include "include/VertexBufferLayout.h"
#include <GLFW/glfw3.h>

Crosshair::Crosshair()
	: m_VAO{}, m_VBO{}
{
	float size = 10.0f;
    m_vertices = {
        -size,  0.0f,
         size,  0.0f,

         0.0f, -size,
         0.0f,  size,
    };

	m_VAO.bind();

	m_VBO.bind();
	m_VBO.setVertices(m_vertices.data(), sizeof(m_vertices));

	VertexBufferLayout layout;
	layout.addLayout(GL_FLOAT, 2, GL_FALSE);

	m_VAO.addBuffer(m_VBO, layout);
}

void Crosshair::render()
{
	m_VAO.bind();
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 4);
}
