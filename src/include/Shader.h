#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include  <unordered_map>


class Shader
{
public:
    void use() const;
    int getUniformLocation(const std::string& name) const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    unsigned int getID() const;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
private:
    unsigned int m_ID;
    std::unordered_map<std::string, int> m_locationCache;
private:
    const char* readFromPath(const char* path);
};

#endif