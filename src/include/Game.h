#pragma once
#include <glm/glm.hpp>
#include "World.h"
#include "Camera.h"
#include "VoxelHandler.h"
#include "Shader.h"
#include "Skybox.h"
#include "Texture.h"
#include "Crosshair.h"
#include "ShaderManager.h"
#include "GLFW/glfw3.h"
#include "Sun.h"

namespace GameDefaults
{
	constexpr int renderDistance = 10;
	constexpr glm::vec3 spawnPoint{0.0f, 15.0f, 0.0f};
	constexpr float screenWidth = 800.0f;
	constexpr float screenHeight = 600.0f;

	inline float getAspectRatio() { return screenWidth / screenHeight; }
}

class Game
{
public:
	Game(GLFWwindow* window);
	~Game();

	void render();

private:
	void loadShaders();
	void updateShaders();
	void processInput();
	void configureWindow();
	void drawDebugAxis();
private:
	World m_world;
	Skybox m_skybox;
	Sun m_sun;

	VoxelHandler m_voxelHandler;
	ShaderManager m_shaderManager;

	GLFWwindow* m_window;

	Crosshair m_crosshair;
	Texture m_atlas;
private:
	static void updateDeltaTime(); 
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
private:
	static inline Camera s_camera		= Camera{GameDefaults::spawnPoint};
	static inline bool s_firstMouse		= true;
	static inline double s_lastMouseX	= GameDefaults::screenWidth / 2;
	static inline double s_lastMouseY	= GameDefaults::screenHeight / 2;
	static inline float s_deltaTime		= 0.0f;
	static inline float s_lastFrameTime = 0.0f;
};