#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (version == 0)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set the size of rendering for openGL, resize viewport as the window resizes
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// CODE:

	// VERTEX SHADER:
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int succes;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		int length;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);

		char* infoLog = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(vertexShader, length, nullptr, infoLog);

		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// FRAGMENT SHADER:
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		int length;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);

		char* infoLog = (char*)alloca(length * sizeof(char));
		glad_glGetShaderInfoLog(fragmentShader, length, nullptr, infoLog);
		
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// SHADER PROGRAM OBJ:
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus)
	{
		int length;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

		char* infoLog = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(shaderProgram, length, nullptr, infoLog);

		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// USE THE PROGRAM OBJ:
	glUseProgram(shaderProgram);

	// DELETE THE NOW NOT USED SHADER OBJECTS
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// BUFFER:
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// UNBIND THE VAO 
	glBindVertexArray(0);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		/* Render here */
		glClearColor(0.2, 0.2, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}