#version 330 core

layout(std140) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos;
};

out vec3 nearPoint;
out vec3 farPoint;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 gridPlane[6] = vec3[](
        vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
        vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
    );
    vec3 p = gridPlane[gl_VertexID];
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, proj);
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, proj);
    gl_Position = vec4(p.xyz, 1.0);
}