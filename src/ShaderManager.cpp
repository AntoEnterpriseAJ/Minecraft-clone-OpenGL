#include "include/ShaderManager.h"

ShaderManager::ShaderManager()
	: m_shaders{}
{
}

void ShaderManager::loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
    if (!m_shaders.contains(shaderName))
    {
        m_shaders[shaderName] = new Shader{vertexPath.c_str(), fragmentPath.c_str()};
    }
}

Shader* ShaderManager::getShader(const std::string& shaderName)
{
	if (!m_shaders.contains(shaderName))
		return nullptr;

	return m_shaders[shaderName];
}
