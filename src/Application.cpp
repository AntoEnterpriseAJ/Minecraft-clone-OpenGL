#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include <string>
#include <iostream>
#include <random>
#include "include/Shader.h"
#include "include/Camera.h"
#include "include/VertexBuffer.h"
#include "include/ElementBuffer.h"
#include "include/VertexBufferLayout.h"
#include "include/VertexArray.h"
#include "include/Chunk.h"
#include "include/World.h"
#include "include/Texture.h"

int initOpenGL();
void processInput(GLFWwindow* window);
float randomf(float lowerBound, float upperBound);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void printCameraStatus();

// SETTINGS
constexpr float screenWidth = 800.0f;
constexpr float screenHeight = 600.0f;

// CAMERA
Camera camera{ glm::vec3(0.0f, 20.0f, 0.0f) };
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

	// SHADER:
	const char* vertexShaderPath = "res/shaders/vertex.vert";
	const char* fragmentShaderPath = "res/shaders/fragment.frag";
	
	Shader shaderProgram(vertexShaderPath, fragmentShaderPath);

	// TEXTURES:
	Texture texture1("res/images/grass.jpg");
	Texture texture2("res/images/img1.jpg");
	Texture texture3("res/images/grass.jpg");

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

	VertexArray groundVA;
	groundVA.bind();

	VertexBuffer groundVB(groundVertices, sizeof(groundVertices));
	groundVB.bind();

	VertexBufferLayout groundLayout;
	groundLayout.addLayout(GL_FLOAT, 3, GL_FALSE);
	groundLayout.addLayout(GL_FLOAT, 2, GL_FALSE);

	groundVA.addBuffer(groundVB, groundLayout);
	groundVA.unbind();

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

	// CUBES:
	VertexArray cubesVA;
	cubesVA.bind();

	VertexBuffer cubesVO(cubesVertices, sizeof(cubesVertices));
	cubesVO.bind();

	VertexBufferLayout cubesLayout;
	cubesLayout.addLayout(GL_FLOAT, 3, GL_FALSE);
	cubesLayout.addLayout(GL_FLOAT, 2, GL_FALSE);

	cubesVA.addBuffer(cubesVO, cubesLayout);
	cubesVA.unbind();

	// RENDER MODE
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// SHADER:
	shaderProgram.use();
	shaderProgram.setInt("ourTexture1", 0);
	shaderProgram.setInt("ourTexture2", 1);

	float currentOpacity = 0.4f;
	shaderProgram.setFloat("opacity", currentOpacity);

	bool wireframe = false;
	World world(8);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		/* Render here */
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// SWITCH RENDER MODE
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		}

		// CAMERA CONTROLS
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::FORWARD, deltaTime);
			printCameraStatus();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::BACKWARD, deltaTime);
			printCameraStatus();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::RIGHT, deltaTime);
			printCameraStatus();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::LEFT, deltaTime);
			printCameraStatus();
		}

		// SET MODEL, VIEW AND PROJECTION MATRICES
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), screenWidth / screenHeight, 0.1f, 100.0f);

		shaderProgram.use();
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		// RENDER THE WORLD
		world.render(shaderProgram);

		// RENDER GROUND
		groundVA.bind();
		glActiveTexture(GL_TEXTURE0);
		texture3.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// RENDER CUBES
		glActiveTexture(GL_TEXTURE0);
		texture1.bind();
		glActiveTexture(GL_TEXTURE1);
		texture2.bind();

		cubesVA.bind();
        
		float opacityIncrement = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			currentOpacity = ((currentOpacity + opacityIncrement) < (1.0f)) ? (currentOpacity + opacityIncrement) : (1.0f);
			shaderProgram.setFloat("opacity", currentOpacity);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			currentOpacity = ((currentOpacity - opacityIncrement) > (0.4f)) ? (currentOpacity - opacityIncrement) : (0.4f);
			shaderProgram.setFloat("opacity", currentOpacity);
		}

		for (int i = 0; i < cubePosCount; ++i)
		{
			cubePositions[i].y = 20.0;

			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(60.0f), rotationAxes[i]);
			
			shaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

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

void printCameraStatus()
{
	std::cout << "Camera at positon: " << "(" << camera.getPositionX() << ", " 
	<< camera.getPositionY() << ", " << camera.getPositionY() << ")" << std::endl;
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
