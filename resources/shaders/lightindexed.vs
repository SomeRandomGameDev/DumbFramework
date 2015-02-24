#version 410 core
layout (location = 0) in vec2 vs_position;

uniform  ivec2 gridsize   = ivec2(32, 24);

void main(void)
{
    vec2 grid;
    grid.y = floor(gl_InstanceID/gridsize.x);
    grid.x = gl_InstanceID - grid.y * gridsize.x; 
    gl_Position = vec4((2.0*grid + vs_position + 1.0)/gridsize - 1.0, 0.0, 1.0);
}

// glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, grid.x*grid.y);
