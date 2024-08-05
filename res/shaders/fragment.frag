#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FacePos;

uniform sampler2D ourTexture1;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    const vec3 lightColor = vec3(0.8, 0.8, 0.8);
    vec3 normal = normalize(Normal);

    float ambientFactor = 0.5 * (normalize(lightPos.y) < 0.5f ? 0.5f : normalize(lightPos.y));
    vec3 ambientLight = lightColor * ambientFactor;

    vec3 lightDirection = normalize(lightPos - FacePos);
    float diffuseFactor = max(dot(lightDirection, normal), 0.0);
    vec3 diffuseLight = diffuseFactor * lightColor;

    vec3 viewDirection = normalize(viewPos - FacePos);
    vec3 reflection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflection), 0.0), 16);
    vec3 specularLight = specularFactor * lightColor;
    
    if (lightPos.y > 0.0f)
    {
        FragColor = texture(ourTexture1, TexCoord) * vec4(ambientLight + diffuseLight + specularLight, 1.0);
    } else FragColor = texture(ourTexture1, TexCoord) * vec4(ambientLight, 1.0);
}