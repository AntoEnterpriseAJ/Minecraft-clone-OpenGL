#pragma once
#include <glm/glm.hpp>
#include "World.h"
#include "Camera.h"
#include "VoxelHandler.h"
#include "Shader.h"
#include "Skybox.h"
#include "Texture.h"
#include "GLFW/glfw3.h"

namespace GameDefaults
{
	constexpr int renderDistance = 10;
	constexpr glm::vec3 spawnPoint{0.0f, 10.0f, 0.0f};
	constexpr float screenWidth = 800.0f;
	constexpr float screenHeight = 600.0f;

	inline float getAspectRatio() { return screenWidth / screenHeight; }
}

class Game
{
public:
	Game(GLFWwindow* window);

	void render();
private:
	void initSkyboxShader();
	void initBlockShader();
	void processInput();
	void configureWindow();
private:
	static inline Camera s_camera		= Camera{GameDefaults::spawnPoint};
	static inline bool s_firstMouse		= true;
	static inline double s_lastMouseX	= GameDefaults::screenWidth / 2;
	static inline double s_lastMouseY	= GameDefaults::screenHeight / 2;
	static inline float s_deltaTime		= 0.0f;
	static inline float s_lastFrameTime = 0.0f;
private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
	static void updateDeltaTime();
	static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
									   GLsizei length, const char* message, const void* userParam);
private:
	World m_world;
	Skybox m_skybox;
	VoxelHandler m_voxelHandler;

	Shader m_crosshairShader;
	Shader m_skyboxShader;
	Shader m_blockShader;

	GLFWwindow* m_window;

	Texture m_atlas;
};