#version 410 core

layout (location=0) in vec3 vs_position;
layout (location=1) in vec2 vs_uv;
layout (location=2) in vec3 vs_normal;
layout (location=3) in vec4 vs_tangent;

layout (std140, binding=0) uniform View
{
    mat4 viewProjMatrix;
};

uniform mat4 modelMatrix;       // [todo] Find a way to do both single and multiple instances
uniform mat3 normalMatrix;      // [todo] Find a way to do both single and multiple instances

out VS_OUT
{
    vec3 worldPos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} vs_out;

void main()
{
    vec4 tmp = modelMatrix * vec4(vs_position, 1.0);
    gl_Position = viewProjMatrix *tmp;
    vs_out.worldPos  = tmp.xyz;
    vs_out.texCoord  = vs_uv;
    vs_out.normal    = normalMatrix * vs_normal;
    vs_out.tangent   = normalMatrix * vs_tangent.xyz;
    vs_out.bitangent = cross(vs_out.normal, vs_out.tangent) * vs_tangent.w;
}
