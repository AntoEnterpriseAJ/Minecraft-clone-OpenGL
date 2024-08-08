#pragma once
#include <string>
#include <vector>
#include <array>
#include "VertexArray.h"
#include "VertexBuffer.h"

class Skybox
{
public:
    Skybox(const std::vector<std::string>& pathToFaces = {
			"res/skybox/right.jpg",
			"res/skybox/left.jpg",
			"res/skybox/top.jpg",
			"res/skybox/bottom.jpg",
			"res/skybox/front.jpg",
			"res/skybox/back.jpg",
		});

	void render() const;

private:
	void genTextures(const std::vector<std::string>& pathToFaces);
	void bindTexture() const;

	unsigned int m_TextureID;

	VertexArray m_VAO;
	VertexBuffer m_VBO;
};