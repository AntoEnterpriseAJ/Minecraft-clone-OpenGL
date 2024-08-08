#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "UIComponent.h"
#include <array>

class Crosshair : public UIComponent 
{
public:
	Crosshair();

	void render() override;

private:
	std::array<float, 8> m_vertices;

	VertexArray m_VAO;
	VertexBuffer m_VBO;
};