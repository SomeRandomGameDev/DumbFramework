#version 420 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec4 vs_pointLightColor;
layout (location = 2) in vec4 vs_pointLightPosition;

layout (std140, binding=1) uniform View
{
    mat4 viewProjMatrix;
    vec3 eye;
};

out VS_OUT
{
    flat vec4 position;
    flat vec4 color;
} pointLight;

void main(void)
{
    vec4 position = viewProjMatrix * vec4(vs_pointLightPosition.w * vs_position + vs_pointLightPosition.xyz, 1.0);
    pointLight.position = vs_pointLightPosition;
    pointLight.color = vs_pointLightColor;
    gl_Position = position;
}
