#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

// Uniform block for view and projection matrices
layout(std140) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos; // Camera position
};

out vec3 FragPos;      // Fragment position in world space
out vec3 Normal;       // Normal vector in world space
out vec2 TexCoords;    // Texture coordinates
out vec3 ViewDir;      // View direction (camera to fragment)

uniform mat4 model;    // Model transformation matrix

void main()
{
    // Calculate the world position of the vertex
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform the normal vector to world space
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass texture coordinates
    TexCoords = aTexCoords;

    // Calculate the view direction (camera position - fragment position)
    ViewDir = normalize(pos - FragPos);

    // Apply projection and view transformations
    gl_Position = proj * view * vec4(FragPos, 1.0);
}
