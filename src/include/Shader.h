#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, float value) const;
    unsigned int getID() const;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
private:
    unsigned int m_ID;
private:
    const char* readFromPath(const char* path);
};

#endif