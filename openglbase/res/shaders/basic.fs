#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 Normal;
in vec2 texCoord;  // Receive the texture coordinates from the vertex shader

uniform sampler2D diffuse;  // Texture sampler

void main()
{
    FragColor = texture(diffuse, texCoord);  // Use texCoord to sample the texture
}