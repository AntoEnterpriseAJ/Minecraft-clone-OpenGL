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
#include "include/VoxelHandler.h"
#include "include/Skybox.h"

int initOpenGL();
void processInput(GLFWwindow* window);
float randomf(float lowerBound, float upperBound);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void printCameraStatus();
void handleCameraControls(GLFWwindow* window);

// WORLD
constexpr float renderDistance = 10.0f;
constexpr float chunkSize = 16.0f;

// SETTINGS
constexpr float screenWidth = 800.0f;
constexpr float screenHeight = 600.0f;

// CAMERA
Camera camera{ glm::vec3(renderDistance * chunkSize / 2.0f, 20.0f, renderDistance * chunkSize / 2.0f) };
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

	initOpenGL();
	/*glfwSwapInterval( 0 );*/

	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	const char* vertexShaderPath = "res/shaders/vertex.vert";
	const char* fragmentShaderPath = "res/shaders/fragment.frag";
	Shader shaderProgram(vertexShaderPath, fragmentShaderPath);
	Shader skyboxShader("res/shaders/skybox.vert", "res/shaders/skybox.frag");
	Shader crosshairShader("res/shaders/crosshair.vert", "res/shaders/crosshair.frag");

	Texture atlasTexture("res/atlas/atlas.png");
	glActiveTexture(GL_TEXTURE0);
	atlasTexture.bind();

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shaderProgram.use();
	shaderProgram.setInt("ourTexture1", 0);
	
	//CROSSHAIR:
	float crosshairVertices[] = {
	   -0.01f,   0.0f ,  0.0f,
		0.01f,   0.0f ,  0.0f,

		0.0f ,  -0.014f,  0.0f,
		0.0f ,   0.014f,  0.0f,
	};

	unsigned int VAOCrosshair, VBOCrosshair;
	glGenVertexArrays(1, &VAOCrosshair);
	glBindVertexArray(VAOCrosshair);

	glGenBuffers(1, &VBOCrosshair);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCrosshair);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);


	bool wireframe = false;
	World world(10);
	VoxelHandler voxelHandler(world, camera.getPosition(), camera.getFront());

	Skybox skybox;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		crosshairShader.use();

		glBindVertexArray(VAOCrosshair);
		glDrawArrays(GL_LINES, 0, 4);


		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		}

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleCameraControls(window);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), screenWidth / screenHeight, 0.1f, 1000.0f);

		shaderProgram.use();
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		atlasTexture.bind();
		world.render(camera.getPosition());
		voxelHandler.rayCast(camera.getPosition(), camera.getFront());

		skyboxShader.use();
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
		skyboxShader.setMat4("projection", projection);

		skybox.render();

		glfwSwapBuffers(window);
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
	<< camera.getPositionY() << ", " << camera.getPositionZ() << ")" << std::endl;
}

void handleCameraControls(GLFWwindow* window)
{
	printCameraStatus();

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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::SPACE, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::SHIFT, deltaTime);
	}
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
