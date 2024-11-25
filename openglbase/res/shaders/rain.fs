#version 330 core

out vec4 FragColor;

uniform float time;
uniform vec2 resolution;
uniform float density; // Controls rain density
uniform float speed;   // Controls rain fall speed

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main()
{
    // Normalize screen coordinates
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv.y = 1.0 - uv.y; // Flip y-axis to match OpenGL convention

    // Offset the UV coordinates to simulate rain falling
    uv.y += time * speed;

    // Create a grid for raindrops
    vec2 grid = floor(uv * density);
    vec2 localUV = fract(uv * density);

    // Randomize raindrop position
    float offset = random(grid);
    localUV.y += offset * 0.5;

    // Shape the raindrop
    float drop = smoothstep(0.0, 0.2, localUV.y) * (1.0 - smoothstep(0.15, 0.2, localUV.y));

    // Apply fade-out effect
    float fade = smoothstep(0.8, 1.0, uv.y);
    drop *= fade;

    // Output the raindrop color with transparency
    FragColor = vec4(vec3(drop), drop);
}
