#version 330 core
out vec4 FragColor;

in vec3 fragPos;

uniform vec3 sourceColor;

void main()
{
    FragColor = vec4(sourceColor, 1.0); // set all 4 vector values to 1.0
}
