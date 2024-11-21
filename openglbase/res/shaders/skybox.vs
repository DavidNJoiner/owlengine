#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 skyboxProjection;
uniform mat4 skyboxView;

void main()
{
    TexCoords = aPos;
    vec4 pos = skyboxProjection * skyboxView * vec4(aPos, 0.0);
    gl_Position = pos.xyww;
}  