#include "include/Game.h"

Game::Game(GLFWwindow* window)
	: m_window{window}, m_world{GameDefaults::renderDistance}, m_skybox{},
	  m_voxelHandler{m_world, s_camera.getPosition(), s_camera.getFront()}, m_atlas{"res/atlas/atlas.png"},
      m_blockShader{"res/shaders/vertex.vert", "res/shaders/fragment.frag"},
	  m_skyboxShader{"res/shaders/skybox.vert", "res/shaders/skybox.frag"},
	  m_crosshairShader{"res/shaders/crosshair.vert", "res/shaders/crosshair.frag"}
{
	configureWindow();
}

void Game::render()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		updateDeltaTime();
		processInput();

		initBlockShader();
		m_atlas.bind();
		m_world.render(s_camera.getPosition());

		m_voxelHandler.rayCast(s_camera.getPosition(), s_camera.getFront());

		initSkyboxShader();
		m_skybox.render();

		glfwSwapBuffers(m_window);
        glfwPollEvents();
	}

	glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Game::initSkyboxShader()
{
	m_skyboxShader.use();

	glm::mat4 view = s_camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(s_camera.getFOV()), GameDefaults::getAspectRatio(), 0.1f, 1000.0f);

	m_skyboxShader.setMat4("view", glm::mat4(glm::mat3(s_camera.getViewMatrix())));
	m_skyboxShader.setMat4("projection", projection);
}

void Game::initBlockShader()
{
	m_blockShader.use();

    glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = s_camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(s_camera.getFOV()), GameDefaults::getAspectRatio(), 0.1f, 1000.0f);

	m_blockShader.setInt("ourTexture1", 0);
	m_blockShader.setMat4("model", model);
	m_blockShader.setMat4("view", view);
	m_blockShader.setMat4("projection", projection);
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

void APIENTRY Game::glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
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

