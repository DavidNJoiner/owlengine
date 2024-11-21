#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout(std140) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos; // Renamed for clarity
};

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
