#include "include/Skybox.h"
#include "glad/glad.h"
#include "stb_image.h"
#include <iostream>
#include "include/VertexBufferLayout.h"


Skybox::Skybox(const std::vector<std::string>& pathToFaces)
	: m_VAO{}, m_VBO{}
{
	genTextures(pathToFaces);

	constexpr std::array<float, 108> skyVertices = {
	    -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
	};

	m_VAO.bind();

    m_VBO.bind();
	m_VBO.setVertices(skyVertices.data(), skyVertices.size() * sizeof(float));

	VertexBufferLayout layouts;
	layouts.addLayout(GL_FLOAT, 3, false);

	m_VAO.addBuffer(m_VBO, layouts);
}

void Skybox::render() const
{
	m_VAO.bind();
	bindTexture();

    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

void Skybox::genTextures(const std::vector<std::string>& pathToFaces)
{
	stbi_set_flip_vertically_on_load(true);
	constexpr int numberOfFaces = 6;

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

	unsigned char* face;
	int width, height, channels;
	for (int i = 0; i < numberOfFaces; ++i)
	{
		face = stbi_load(pathToFaces[i].c_str(), &width, &height, &channels, 0);
		if (!face)
		{
			std::cout << "WARNING: unable to load cubemap face at path: " << pathToFaces[i] << std::endl;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, face);
		stbi_image_free(face);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Skybox::bindTexture() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}
