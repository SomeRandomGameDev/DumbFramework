#version 410 core

// [todo] use sampleArray?
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

in VS_OUT
{
    vec3 worldPos;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} fs_in;

layout (location=0) out vec4 albedo;
layout (location=1) out vec4 specular;
layout (location=2) out vec4 normal;
layout (location=3) out vec4 world;

void main()
{
    mat3 tbn = mat3(fs_in.tangent, fs_in.bitangent, fs_in.normal);
    vec3 n = 2.0*texture(normalMap, fs_in.texCoord).xyz - 1.0;
    n = normalize(tbn * n);
    
    albedo   = texture(diffuseMap,  fs_in.texCoord);
    specular = texture(specularMap, fs_in.texCoord);
    normal   = vec4(n, 1.0f);
    world    = vec4(fs_in.worldPos, 1.0f);
}
