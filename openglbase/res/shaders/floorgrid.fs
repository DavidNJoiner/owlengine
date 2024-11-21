#version 330 core

layout(std140) uniform ViewUniforms {
    mat4 view;
    mat4 proj;
    vec3 pos;
};

in vec3 nearPoint;
in vec3 farPoint;
out vec4 outColor;

uniform float near; // Near plane distance
uniform float far;  // Far plane distance

vec4 grid(vec3 fragPos3D, float scale) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    vec4 color = vec4(0.1, 0.1, 0.1, 1 - min(line, 0.9));

    float axisWidth = 0.05; // Adjust this value to change the thickness of the axes
    
    if (abs(fragPos3D.x) < axisWidth * derivative.x)
        color = vec4(0.0, 0.0, 1.0, 1.0); // Primary blue for Z-axis
    if (abs(fragPos3D.z) < axisWidth * derivative.y)
        color = vec4(1.0, 0.0, 0.0, 1.0); // Primary red for X-axis

    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = proj * view * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(vec3 pos) {
    float clip_space_depth = computeDepth(pos) * 2.0 - 1.0; // Normalize between -1 and 1
    float linearDepth = (18.0 * near * far) / (far + near - clip_space_depth * (far - near));
    return linearDepth / far; // Normalize
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    
    if (t > 0.0 && t < 1.0) { // Ensure valid intersection
        vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
        gl_FragDepth = computeDepth(fragPos3D);

        float linearDepth = computeLinearDepth(fragPos3D);
        float fading = max(0.0, (0.5 - linearDepth));

        outColor = grid(fragPos3D, 10.0) + grid(fragPos3D, 1.0); // Multiple resolutions
        outColor.a *= fading; // Apply fading effect
    } else {
        discard; // Discard fragments where the plane does not intersect
    }
}