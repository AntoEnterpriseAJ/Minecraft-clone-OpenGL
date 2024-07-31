#pragma once
#include "Shader.h"
#include <unordered_map>
#include <string>

class ShaderManager
{
public:
	ShaderManager();

	void loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	Shader* getShader(const std::string& shaderName);

private:
	std::unordered_map<std::string, Shader*> m_shaders;
};