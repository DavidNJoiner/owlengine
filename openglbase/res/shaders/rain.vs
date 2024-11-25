#version 330 core
void main()
{
    vec2 positions[4] = vec2[](
        vec2(-1.0,  1.0), // Top-left
        vec2(-1.0, -1.0), // Bottom-left
        vec2( 1.0, -1.0), // Bottom-right
        vec2( 1.0,  1.0)  // Top-right
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
