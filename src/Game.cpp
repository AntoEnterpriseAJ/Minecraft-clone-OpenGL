#include "include/Game.h"

Game::Game(GLFWwindow* window)
	: m_window{window}, m_world{GameDefaults::renderDistance}, m_skybox{}, m_sun{}, 
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

		drawDebugAxis();

		m_shaderManager.getShader("crosshairShader")->use();
		m_crosshair.render();

		m_shaderManager.getShader("blockShader")->use();
		m_atlas.bind();
		m_world.render(s_camera.getPosition());
		m_voxelHandler.rayCast(s_camera.getPosition(), s_camera.getFront());

		m_sun.updatePosition(s_camera.getPosition(), s_deltaTime);
		m_sun.render(m_shaderManager.getShader("sunShader"));

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
	m_shaderManager.loadShader("sunShader", "res/shaders/sun.vert", "res/shaders/sun.frag");
}

void Game::updateShaders()
{
	Shader* blockShader = m_shaderManager.getShader("blockShader");
	Shader* skyboxShader = m_shaderManager.getShader("skyboxShader");
	Shader* crosshairShader = m_shaderManager.getShader("crosshairShader");
	Shader* sunShader = m_shaderManager.getShader("sunShader");

	glm::mat4 view = s_camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(s_camera.getFOV()), GameDefaults::getAspectRatio(), 0.1f, 1000.0f);

	glm::mat4 model{1.0f};
	model = glm::translate(model, m_sun.getPosition());
	model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
	sunShader->use();
	sunShader->setMat4("model", model);
	sunShader->setMat4("view", view);
	sunShader->setMat4("projection", projection);

	model = glm::mat4(1.0f);
	glm::vec3 sunPosition = m_sun.getPosition();
	glm::vec3 cameraPos = s_camera.getPosition();
	blockShader->use();
	blockShader->setInt("ourTexture1", 0);
	blockShader->setMat4("model", model);
	blockShader->setMat4("view", view);
	blockShader->setMat4("projection", projection);
	blockShader->setVec3("lightPos", sunPosition.x, sunPosition.y, sunPosition.z);
	blockShader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	skyboxShader->use();
	skyboxShader->setMat4("view", glm::mat4(glm::mat3(s_camera.getViewMatrix())));
	skyboxShader->setMat4("projection", projection);
	skyboxShader->setVec3("lightPos", sunPosition.x, sunPosition.y, sunPosition.z);

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

void Game::drawDebugAxis()
{
	float axisVertices[] = {
		//x+ axis
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		//x- axis
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		//y+ axis
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		//y- axis
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		//z+ axis
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,

		//z- axis
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
	};

	Shader axisShader{"res/shaders/axis.vert", "res/shaders/axis.frag"};
	axisShader.use();

	unsigned int axisVAO, axisVBO;
	glGenVertexArrays(1, &axisVAO);
	glBindVertexArray(axisVAO);

	glGenBuffers(1, &axisVBO);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glm::mat4 model{1.0f};
	//model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
	model = glm::scale(model, glm::vec3(200.0f, 200.0f, 200.0f));
	glm::mat4 view = s_camera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(s_camera.getFOV()), GameDefaults::getAspectRatio(), 0.1f, 1000.0f);

	axisShader.setMat4("model", model);
	axisShader.setMat4("view", view);
	axisShader.setMat4("projection", projection);

	glLineWidth(3.0f);
	glBindVertexArray(axisVAO);
	axisShader.setVec3("color", 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_LINES, 0, 2);
	axisShader.setVec3("color", 0.2f, 0.0f, 0.0f);
	glDrawArrays(GL_LINES, 2, 2);
	axisShader.setVec3("color", 0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_LINES, 4, 2);
	axisShader.setVec3("color", 0.0f, 0.2f, 0.0f);
	glDrawArrays(GL_LINES, 6, 2);
	axisShader.setVec3("color", 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 8, 2);
	axisShader.setVec3("color", 0.0f, 0.0f, 0.2f);
	glDrawArrays(GL_LINES, 10, 2);
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

