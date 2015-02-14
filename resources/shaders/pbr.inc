#define PI 3.1415926535897932384626433832795

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
