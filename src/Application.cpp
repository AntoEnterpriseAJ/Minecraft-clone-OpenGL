#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include "include/Shader.h"

int initOpenGL();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// INIT OPENGL UTIL FUNCTION
	initOpenGL();

	// set the size of rendering for openGL, resize viewport as the window resizes
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// CODE:
	
	// SHADER:
	const char* vertexShaderPath = "res/shaders/vertex.vert";
	const char* fragmentShaderPath = "res/shaders/fragment.frag";
	
	Shader shaderProgram(vertexShaderPath, fragmentShaderPath);

	// LOAD IMAGE1:
	int width, height, nrChannels;
	unsigned char* imgData1 = stbi_load("res/images/img2.jpg", &width, &height, &nrChannels, 0);
	if (!imgData1)
	{
		std::cout << "Failed to load texture\n";
	}

	// LOAD IMAGE2:
	int width2, height2, nrChannels2;
	unsigned char* imgData2 = stbi_load("res/images/img1.jpg", &width2, &height2, &nrChannels2, 0);
	if (!imgData2)
	{
		std::cout << "Failed to load texture\n";
	}

	// CREATE TEXTURE2:
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData2);
	glGenerateMipmap(GL_TEXTURE_2D);


	// CREATE TEXTURE1:
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData1);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	// FREE THE IMAGE MEMORY:
	stbi_image_free(imgData1);
	stbi_image_free(imgData2);

	// BUFFER:
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		2, 1, 0,
		0, 3, 2
	}; 

	// VAO (VERTEX ARRAY OBJECT):
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO (VERTEX BUFFER OBJECT):
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // colors
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // tex coords

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	// EBO (ELEMENT BUFFER OBJECT)
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// UNBIND THE VAO 
	glBindVertexArray(0);

	// RENDER MODE
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// SHADER:
	shaderProgram.use();
	shaderProgram.setInt("ourTexture", 0);
	shaderProgram.setInt("ourTexture2", 1);

	int opacityLocation = glGetUniformLocation(shaderProgram.getID(), "opacity");
	float currentOpacity = 0.0f;
	glUniform1f(opacityLocation, currentOpacity);

	/* Loop until the user closes the window */
	float xValue = 0.0f;
	float offset = 0.005f;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		/* Render here */
		glClearColor(0.2, 0.2, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);	
		

		//int uniformLocation = glGetUniformLocation(shaderProgram.getID(), "offset");
		//glUniform3f(uniformLocation, xValue, 0.0f, 0.0f);

		shaderProgram.setVec3("offset", xValue, 0.0f, 0.0f);

		if (xValue + offset > 0.5f || xValue + offset < -0.5f)
		{
			offset = -offset;
		}
		xValue += offset;

		float opacityIncrement = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			glUniform1f(opacityLocation, (currentOpacity < 1.0f) ? (currentOpacity + opacityIncrement) : 1.0f);
			currentOpacity = currentOpacity + opacityIncrement;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			glUniform1f(opacityLocation, (currentOpacity > 0.0f) ? (currentOpacity - opacityIncrement) : 0.0f);
			currentOpacity = currentOpacity - opacityIncrement;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	// Cleanup
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &texture2);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}