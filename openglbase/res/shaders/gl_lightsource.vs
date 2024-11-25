#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

layout(std140) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos;
};

out vec3 fragPos;

void main()
{
	fragPos = aPos;
	gl_Position = proj * view * model * vec4(aPos, 1.0);
}
