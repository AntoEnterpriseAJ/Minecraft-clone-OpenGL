#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <random>
#include "include/Shader.h"
#include "include/Camera.h"
#include "include/VertexBuffer.h"
#include "include/ElementBuffer.h"


int initOpenGL();
void processInput(GLFWwindow* window);
float randomf(float lowerBound, float upperBound);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

// SETTINGS
constexpr float screenWidth = 800.0f;
constexpr float screenHeight = 600.0f;

// CAMERA
Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f) };
float lastMouseX = screenWidth / 2.0f;
float lastMouseY = screenHeight / 2.0f;
bool firstMouse = true;

// DELTA TIME
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	window = glfwCreateWindow(screenWidth, screenHeight, "Test", NULL, NULL);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// INIT OPENGL UTIL FUNCTION
	initOpenGL();

	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// CODE:
	
	// SHADER:
	const char* vertexShaderPath = "res/shaders/vertex.vert";
	const char* fragmentShaderPath = "res/shaders/fragment.frag";
	
	Shader shaderProgram(vertexShaderPath, fragmentShaderPath);

	//STB
	stbi_set_flip_vertically_on_load(true);

	// LOAD IMAGE1:
	int width, height, nrChannels;
	unsigned char* imgData1 = stbi_load("res/images/highResBox.jpg", &width, &height, &nrChannels, 0);
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

	// LOAD IMAGE3:
	int width3, height3, nrChannels3;
	unsigned char* imgData3 = stbi_load("res/images/ground.jpg", &width3, &height3, &nrChannels3, 0);
	if (!imgData3)
	{
		std::cout << "Failed to load texture\n";
	}

	// CREATE TEXTURE1:
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData1);
	glGenerateMipmap(GL_TEXTURE_2D);

	// CREATE TEXTURE2:
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData2);
	glGenerateMipmap(GL_TEXTURE_2D);

	// CREATE TEXTURE3:
	unsigned int texture3;
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData3);
	glGenerateMipmap(GL_TEXTURE_2D);

	// FREE THE IMAGES MEMORY:
	stbi_image_free(imgData1);
	stbi_image_free(imgData2);
	stbi_image_free(imgData3);

	// GROUND
	float groundVertices[] = {
		// positions          // texture coords
		-50.0f, -0.5f,  50.0f,  0.0f,  0.0f,
		 50.0f, -0.5f,  50.0f,  50.0f, 0.0f,
		 50.0f, -0.5f, -50.0f,  50.0f, 50.0f,
		 50.0f, -0.5f, -50.0f,  50.0f, 50.0f,
		-50.0f, -0.5f, -50.0f,  0.0f,  50.0f,
		-50.0f, -0.5f,  50.0f,  0.0f,  0.0f
	};

	unsigned int groundVAO;
	glGenVertexArrays(1, &groundVAO);
	glBindVertexArray(groundVAO);

	VertexBuffer groundVB(groundVertices, sizeof(groundVertices));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// CUBES:
	float cubesVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	constexpr unsigned int cubePosCount = 10;
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 rotationAxes[cubePosCount];
	for (int i = 0; i < cubePosCount; ++i)
	{
		rotationAxes[i] = glm::vec3(randomf(0.0f, 0.8f), randomf(0.0f, 0.8f), randomf(0.0f, 0.8f));
	}

	// VAO (VERTEX ARRAY OBJECT):
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);	

	// VBO (VERTEX BUFFER OBJECT):
	VertexBuffer cubesVBO(cubesVertices, sizeof(cubesVertices));
	cubesVBO.bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // positions
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // tex coords
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// UNBIND THE VAO 
	glBindVertexArray(0);

	// RENDER MODE
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// SHADER:
	shaderProgram.use();
	shaderProgram.setInt("ourTexture", 0);
	shaderProgram.setInt("ourTexture2", 1);

	int opacityLocation = glGetUniformLocation(shaderProgram.getID(), "opacity");
	float currentOpacity = 0.0f;
	glUniform1f(opacityLocation, currentOpacity);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		/* Render here */
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// GROUND
		glBindVertexArray(groundVAO);
		glm::mat4 model(1.0f);
		unsigned int modelLocation = glGetUniformLocation(shaderProgram.getID(), "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// CUBES
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
			
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

		// CAMERA CONTROLS:
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::RIGHT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::LEFT, deltaTime);
		}

		//GLM MATHS:
		for (int i = 0; i < cubePosCount; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(60.0f), rotationAxes[i]);

			int modelLoc = glGetUniformLocation(shaderProgram.getID(), "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glm::mat4 view;
			view = camera.getViewMatrix();

		    int viewLoc = glGetUniformLocation(shaderProgram.getID(), "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(camera.getFOV()), screenWidth / screenHeight, 0.1f, 100.0f);
			
			int projectionLoc = glGetUniformLocation(shaderProgram.getID(), "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	// Cleanup
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &texture2);
	glDeleteTextures(1, &texture3);

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &groundVAO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastMouseX = xPos;
		lastMouseY = yPos;
		firstMouse = false;	
	}

	float yawOffset = xPos - lastMouseX;
	float pitchOffset = lastMouseY - yPos;

	lastMouseX = xPos;
	lastMouseY = yPos;

	camera.processMouseCursor(yawOffset, pitchOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(yOffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

float randomf(float lowerBound, float upperBound)
{
	std::random_device rd{};
	std::mt19937 mt{ rd() };

	std::uniform_real_distribution<float> dis(lowerBound, upperBound);
	return dis(mt);
}
