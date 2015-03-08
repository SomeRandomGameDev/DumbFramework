#version 420 core
#define PI 3.1415926535897932384626433832795

layout (binding=0) uniform sampler2DArray gbuffer;
layout (std140, binding=1) uniform View
{
    mat4 viewProjMatrix;
    vec3 eye;
};

layout (location=0) out vec4 color_out;

in VS_OUT
{
    flat vec4 color;
    flat vec4 position;
    flat vec4 direction;
} spotLight;

float SchlickFresnel(float u)
{
    float m = max(1.0-u, 0.0);
    float m2 = m*m;
    return m2*m2*m;
}

float GTR2(float ndoth, float a)
{
    float a2 = a * a;
    float t  = ndoth*ndoth*(a2-1.0) + 1.0;
    return a2 / (PI * t*t);
}

float G1V(float v, float k)
{
    return v/(v*(1.0-k) + k);
}

vec3 brdf(float dotNV, float dotNL, float dotNH, float dotLH, float dotVH, vec3 diffColor, vec4 specColor)
{        

    if(dotNV < 0 || dotNL < 0)
    { return vec3(0.0); }

    float roughness    = specColor.a;
    float sqrRoughness = roughness * roughness;
    
    // Diffuse
    float fl   = SchlickFresnel(dotNL);
    float fv   = SchlickFresnel(dotNV);
    float fd90 = 0.5 + 2.0*dotLH*dotLH*roughness;
    float fd   = mix(1.0, fd90, fl) * mix(1.0, fd90, fv);
    vec3 diffuse = fd * diffColor / PI;

    // Specular
    float k  = (roughness+1.0) * (roughness+1.0) / 8.0;
    float fh = SchlickFresnel(dotVH);
    vec3  fresnel      = mix(specColor.rgb, vec3(1.0), fh);
    float visibility   = G1V(dotNL, k) * G1V(dotNV, k);
    float distribution = GTR2(dotNH, sqrRoughness);
    vec3 specular  = fresnel * visibility * distribution;

    return (specular + diffuse);
}

void main(void)
{
    vec3 position = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 3), 0).xyz;
    vec4  lightPosition = spotLight.position;
    float lightDistance = distance(position, lightPosition.xyz);
    // [todo] Compute the angle between the lit point and the light direction ray.
    if(lightDistance > lightPosition.w)
    {
        discard;
    }
    vec3 albedo   = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 0), 0).xyz;
    vec4 specular = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 1), 0);
    vec3 normal   = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 2), 0).xyz;
    vec3 view     = normalize(eye - position);
    float dotNV = dot(view, normal);
    
    vec3 light = normalize(lightPosition.xyz - position);
    vec3 halfL = normalize(light + view);
    float dotNL = dot(light, normal);
    float dotNH = dot(halfL, normal);
    float dotVH = dot(halfL, view);
    float dotLH = dot(light, halfL);
    
    float attNum = clamp(1.0 - pow(lightDistance/lightPosition.w, 4), 0.0, 1.0);
    float attenuation   = attNum * attNum / (lightDistance*lightDistance + 1.0);

    color_out = vec4(clamp(brdf(dotNV, dotNL, dotNH, dotLH, dotVH, albedo, specular) * spotLight.color.rgb * dotNL * attenuation, 0.0, 1.0), 1.0);
}
