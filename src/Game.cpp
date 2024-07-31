#include "include/Game.h"

Game::Game(GLFWwindow* window)
	: m_window{window}, m_world{GameDefaults::renderDistance}, m_skybox{},
	  m_voxelHandler{m_world, s_camera.getPosition(), s_camera.getFront()}, m_atlas{"res/atlas/atlas.png"}
{
	configureWindow();
	loadShaders();
}

Game::~Game()
{
	glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Game::render()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		updateShaders();
		updateDeltaTime();
		processInput();

		m_shaderManager.getShader("crosshairShader")->use();
		m_crosshair.render();

		m_shaderManager.getShader("blockShader")->use();
		m_atlas.bind();
		m_world.render(s_camera.getPosition());

		m_voxelHandler.rayCast(s_camera.getPosition(), s_camera.getFront());

		m_shaderManager.getShader("skyboxShader")->use();
		m_skybox.render();

		glfwSwapBuffers(m_window);
        glfwPollEvents();
	}
}

void Game::loadShaders()
{
	m_shaderManager.loadShader("blockShader", "res/shaders/vertex.vert", "res/shaders/fragment.frag");
	m_shaderManager.loadShader("skyboxShader", "res/shaders/skybox.vert", "res/shaders/skybox.frag");
	m_shaderManager.loadShader("crosshairShader", "res/shaders/crosshair.vert", "res/shaders/crosshair.frag");
}

void Game::updateShaders()
{
	Shader* blockShader = m_shaderManager.getShader("blockShader");
	Shader* skyboxShader = m_shaderManager.getShader("skyboxShader");
	Shader* crosshairShader = m_shaderManager.getShader("crosshairShader");

	glm::mat4 model{1.0f};
	glm::mat4 view = s_camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(s_camera.getFOV()), GameDefaults::getAspectRatio(), 0.1f, 1000.0f);

	blockShader->use();
	blockShader->setInt("ourTexture1", 0);
	blockShader->setMat4("model", model);
	blockShader->setMat4("view", view);
	blockShader->setMat4("projection", projection);

	skyboxShader->use();
	skyboxShader->setMat4("view", glm::mat4(glm::mat3(s_camera.getViewMatrix())));
	skyboxShader->setMat4("projection", projection);

	int width, height;
	glfwGetWindowSize(m_window, &width, &height);

	crosshairShader->use();
	crosshairShader->setVec2("uWindowSize", width, height);
}

void Game::processInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	//printCameraStatus();

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        s_camera.startSprinting();
    }
	else s_camera.stopSprinting();

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		s_camera.processKeyboard(Camera::FORWARD, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		s_camera.processKeyboard(Camera::BACKWARD, s_deltaTime);
	} 
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		s_camera.processKeyboard(Camera::RIGHT, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		s_camera.processKeyboard(Camera::LEFT, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		s_camera.processKeyboard(Camera::UP, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		s_camera.processKeyboard(Camera::DOWN, s_deltaTime);
	}
}

void Game::configureWindow()
{
	glfwSwapInterval(0);

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
}

void Game::updateDeltaTime()
{
	float currentFrameTime = glfwGetTime();
	s_deltaTime = currentFrameTime - s_lastFrameTime;
	s_lastFrameTime = currentFrameTime;
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (s_firstMouse)
	{
		s_lastMouseX = xPos;
		s_lastMouseY = yPos;
		s_firstMouse = false;	
	}

	float yawOffset = xPos - s_lastMouseX;
	float pitchOffset = s_lastMouseY - yPos;

	s_lastMouseX = xPos;
	s_lastMouseY = yPos;

	s_camera.processMouseCursor(yawOffset, pitchOffset);
}

void Game::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	s_camera.processMouseScroll(yOffset);
}

