#version 420 core

layout (std140, binding=1) uniform View
{
    mat4 viewProjMatrix;
    vec3 eye;
};

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec4 vs_spotLightColor;
layout (location = 2) in vec4 vs_spotLightPosition;
layout (location = 3) in vec4 vs_spotLightDirection;

out VS_OUT
{
    flat vec4 color;
    flat vec4 position;
    flat vec4 direction;
} spotLight;

void main(void)
{
    // [todo] build rotation from spotLight.Direction.xyz
    vec4 position = viewProjMatrix * vec4(vs_spotLightPosition.w * vs_position + vs_spotLightPosition.xyz, 1.0);
    spotLight.position  = vs_spotLightPosition;
    spotLight.color     = vs_spotLightColor;
    spotLight.direction = vs_spotLightDirection;
}
