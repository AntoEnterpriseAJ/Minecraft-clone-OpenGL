#pragma once
#include <array>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"

class Sun
{
public:
	Sun();

	void updatePosition(glm::vec3 cameraPosition, float deltaTime);
	void render(Shader* shader);
	glm::vec3 getPosition() const;

private:
	glm::vec3 m_worldPosition;

	VertexArray m_VAO;
	ElementBuffer m_EBO;
	VertexBuffer m_VBO;
private:
	static constexpr unsigned int s_sunIndices[] = {
        0 , 1 , 2 , 2 , 3 , 0 ,
		4 , 5 , 6 , 6 , 7 , 4 ,
		8 , 9 , 10, 10, 11, 8 ,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20,
	};

	static constexpr float s_sunVertices[] = {
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		                    
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		                    
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		                    
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		                    
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		                    
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
	};
};