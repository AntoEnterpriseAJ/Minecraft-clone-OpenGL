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

void Game::gravity()
{
	glm::vec3 cameraPos = s_camera.getPosition();

	constexpr float fallSpeed = 0.03f;
	const Block& blockBelow = m_world.getBlockAt(cameraPos.x, cameraPos.z, cameraPos.y - fallSpeed);

	if (blockBelow.getType() == Block::Type::AIR)
	{
		s_camera.getPositionRef().y -= fallSpeed;
	}
}

std::array<bool, Camera::Movement::COUNT> Game::getMovementValidDirections()
{
    glm::vec3 cameraPos = s_camera.getPosition();
    constexpr float precision = 0.2f;

    glm::vec3 front = glm::vec3(glm::sign(s_camera.getFront().x), 0.0f, glm::sign(s_camera.getFront().z));
    glm::vec3 right = glm::vec3(glm::sign(s_camera.getRight().x), 0.0f, glm::sign(s_camera.getRight().z));
    glm::vec3 back = -front;
    glm::vec3 left = -right;
    glm::vec3 up = s_camera.getUp();
    glm::vec3 down = -up;

    auto adjustPosition = [](const glm::vec3& pos) -> glm::vec3 {
        return glm::vec3(glm::round(pos.x), glm::round(pos.y), glm::round(pos.z));
    };

    glm::vec3 frontPos = adjustPosition(cameraPos + front * precision);
    glm::vec3 backPos = adjustPosition(cameraPos + back * precision);
    glm::vec3 rightPos = adjustPosition(cameraPos + right * precision);
    glm::vec3 leftPos = adjustPosition(cameraPos + left * precision);
    glm::vec3 upPos = adjustPosition(cameraPos + up * precision);
    glm::vec3 downPos = adjustPosition(cameraPos + down * precision);

    const Block& blockFront = m_world.getBlockAt(frontPos.x, frontPos.z, frontPos.y);
    const Block& blockBack = m_world.getBlockAt(backPos.x, backPos.z, backPos.y);
    const Block& blockRight = m_world.getBlockAt(rightPos.x, rightPos.z, rightPos.y);
    const Block& blockLeft = m_world.getBlockAt(leftPos.x, leftPos.z, leftPos.y);
    const Block& blockUp = m_world.getBlockAt(upPos.x, upPos.z, upPos.y);
    const Block& blockDown = m_world.getBlockAt(downPos.x, downPos.z, downPos.y);

    std::array<bool, Camera::Movement::COUNT> validDirections;
    validDirections[Camera::FORWARD] = blockFront.getType() == Block::Type::AIR;
    validDirections[Camera::BACKWARD] = blockBack.getType() == Block::Type::AIR;
    validDirections[Camera::RIGHT] = blockRight.getType() == Block::Type::AIR;
    validDirections[Camera::LEFT] = blockLeft.getType() == Block::Type::AIR;
    validDirections[Camera::UP] = blockUp.getType() == Block::Type::AIR;
    validDirections[Camera::DOWN] = blockDown.getType() == Block::Type::AIR;

    return validDirections;
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
	std::array<bool, Camera::Movement::COUNT> validDirections = getMovementValidDirections();

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	gravity();

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		s_camera.startSprinting();
	}
	else s_camera.stopSprinting();

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS && validDirections[Camera::FORWARD])
	{
		s_camera.processKeyboard(Camera::FORWARD, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS && validDirections[Camera::BACKWARD])
	{
		s_camera.processKeyboard(Camera::BACKWARD, s_deltaTime);
	} 
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS && validDirections[Camera::RIGHT])
	{
		s_camera.processKeyboard(Camera::RIGHT, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS && validDirections[Camera::LEFT])
	{
		s_camera.processKeyboard(Camera::LEFT, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS && validDirections[Camera::UP])
	{
		s_camera.processKeyboard(Camera::UP, s_deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && validDirections[Camera::DOWN])
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

