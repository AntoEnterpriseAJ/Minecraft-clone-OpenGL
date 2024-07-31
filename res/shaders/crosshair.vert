#version 330 core

layout (location = 0) in vec2 aPos;
uniform vec2 uWindowSize;

void main()
{
    vec2 pos = aPos / uWindowSize;
    gl_Position = vec4(pos, 0.0f, 1.0f);
}