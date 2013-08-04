#version 330 core

precision highp float;
precision highp int;

layout (std140) uniform Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emissive;
} material;

layout (location = 0) out vec4 fragData;

void main() 
{
	fragData = material.ambient;
}
