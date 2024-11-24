#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

layout(std140) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos;
};

void main()
{
    gl_Position = proj * view * vec4(aPos, 1.0);
}