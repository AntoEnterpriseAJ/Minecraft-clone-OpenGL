#include "include/Shader.h"


const char* Shader::readFromPath(const char* path)
{
	std::ifstream fin(path);
	if (!fin.is_open())
	{
		std::cout << "can't open file at: " << path << "\n";
		return nullptr;
	}

	std::stringstream ss;
	ss << fin.rdbuf();
	fin.close();
	
	std::string source = ss.str();
	char* shaderSource = new char[source.size() + 1]; // +1 for null terminator

	// STRCPY IS UNSAFE
	for (int i = 0; i <= source.size(); i++)
	{
		shaderSource[i] = source[i];
	}

	return shaderSource;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: m_ID{ 0 }
{
	const char* vertexSource = readFromPath(vertexPath);
	const char* fragmentSource = readFromPath(fragmentPath);

	m_ID = glCreateProgram();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// VERTEX:
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	// ERROR HANDLING:
	int status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		char logMessage[1024];
		glGetShaderInfoLog(vertexShader, 1024, nullptr, logMessage);

		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << logMessage << std::endl;

		glDeleteProgram(m_ID);
	}

	// FRAGMENT:
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	// ERROR HANDLING:
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		char logMessage[1024];
		glGetShaderInfoLog(fragmentShader, 1024, nullptr, logMessage);

		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << logMessage << std::endl;

		glDeleteProgram(m_ID);
	}

	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);
	glLinkProgram(m_ID);

	glDeleteShader(vertexShader), glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::use() const
{
	glUseProgram(m_ID);
}

int Shader::getUniformLocation(const std::string& name) const
{
	if (m_locationCache.contains(name))
		return m_locationCache.at(name);

	return glGetUniformLocation(m_ID, name.c_str());
}

void Shader::setBool(const std::string& name, bool value) const
{	
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setVec4(const std::string& name, float value) const
{
	glUniform4f(getUniformLocation(name), value, value, value, value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::getID() const
{
	return m_ID;
}
