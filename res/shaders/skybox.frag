#version 330 core

out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skybox;
uniform vec3 lightPos;

void main()
{
    float ambientFactor = 1.0 * (normalize(lightPos.y) < 0.5f ? 0.5f : normalize(lightPos.y));
	fragColor = texture(skybox, texCoords) * ambientFactor;
}