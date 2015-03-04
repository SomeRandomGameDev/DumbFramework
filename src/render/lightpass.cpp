#include <DumbFramework/render/lightpass.hpp>
#include "light/occluders.hpp"

namespace Framework {
namespace Render    {

static const char* g_spotLightVertexShader = R"EOT(
#version 410 core

// [todo] spot and directional light
layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec4 vs_pointLightPosition;
layout (location = 2) in vec4 vs_pointLightColor;
layout (std140, binding=1) uniform View
{
    mat4 viewProjMatrix;
    vec3 eye;
};
out flat VS_OUT
{
    vec4 position;
    vec4 color;
} pointLight;
void main(void)
{
    vec4 position = viewProjMatrix * vec4(vs_pointLightPosition.w * vs_position + vs_pointLightPosition.xyz, 1.0);
    pointLight.position = vs_pointLightPosition;
    pointLight.color = vs_pointLightColor;
    gl_Position = position;
}
)EOT";

// [todo] one per light type
static const char* g_spotLightFragmentShader = R"EOT(
#version 410 core
#define PI 3.1415926535897932384626433832795
layout (binding=0) uniform sampler2DArray gbuffer;
layout (std140, binding=1) uniform View
{
    mat4 viewProjMatrix;
    vec3 eye;
};
layout (location=0) out vec4 color_out;
in flat VS_OUT
{
    vec4 position;
    vec4 color;
} pointLight;

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
    vec4  lightPosition = pointLight.position;
    float lightDistance = distance(position, lightPosition.xyz);
    if(lightDistance <= lightPosition.w)
    {
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

        color_out = vec4(clamp(brdf(dotNV, dotNL, dotNH, dotLH, dotVH, albedo, specular) * pointLight.color.rgb * dotNL * attenuation, 0.0, 1.0), 1.0);
    }
    else
    {
        discard;
    }
}
)EOT";

/**
 * Default constructor.
 */
LightPass::LightPass()
    : _gbuffer(nullptr)
    , _depthbuffer(nullptr)
    , _program()
    , _framebuffer(0)
    , _output()
    , _view()
    , _occludersVertexBuffer()
    , _occludersIndexBuffer()
{}
/**
 * Destructor.
 */
LightPass::~LightPass()
{
    destroy();
}
/**
 * Create light pass.
 * @param [in] gbuffer     Geometry pass output.
 * @param [in] depthbuffer Depth buffer build by geometry pass.
 * @return false if an error occured.
 */
bool LightPass::create(Texture2D* gbuffer, Renderbuffer* depthbuffer)
{
// [todo] Cut init into smaller methods (one for textures, one for buffer...)
    bool ret;
    // [todo] Sanity check
    _gbuffer = gbuffer;
    _depthbuffer = depthbuffer;

    ret = _output.create(gbuffer->size(), Texture::PixelFormat::RGBA_32F);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create light pass output texture.");
        return false;
    }
    _output.bind();
        _output.setMagFilter(Render::Texture::MagFilter::NEAREST_TEXEL);
        _output.setMinFilter(Render::Texture::MinFilter::NEAREST_TEXEL);
        _output.setWrap(Render::Texture::Wrap::CLAMP_TO_EDGE, Render::Texture::Wrap::CLAMP_TO_EDGE);
    _output.unbind();

    glGenFramebuffers(1, &_framebuffer);
    GLenum err = glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Module::Render, (const char*)gluErrorString(err));
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _output.id(), 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer->id());
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE != status)
    {
        GLenum err = glGetError();
        Log_Error(Module::Render, "%s %x",(const char*)gluErrorString(err), status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    ret = createOccluders();
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create occluders.");
        return false;
    }
    clear();
    
    ret = _program.create( {{Render::Shader::Type::VERTEX_SHADER,   g_spotLightVertexShader  },
                            {Render::Shader::Type::FRAGMENT_SHADER, g_spotLightFragmentShader}} );
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create program.");
        return false;
    }

    ret = _program.link();
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to link program.");
        return false;
    }
    
    ret = _view.create(2 * sizeof(float[16]), nullptr, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create point view matrices buffer.");
        return false;
    }
    
    return true;
}

bool LightPass::createOccluders()
{
    bool ret;
    std::array<Geometry::Attribute, LightType::COUNT> occluderAttributes;
    ret = Light::createOccluders(_occludersVertexBuffer, _occludersIndexBuffer, occluderAttributes);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create occluders.");
        return false;
    }

    for(size_t i=0; i<LightType::COUNT; i++)
    {
        ret = _buffer[i].create(LightType::maxCount[i] * LightType::elementCount[i] * sizeof(float), nullptr, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
        if(false == ret)
        {
            Log_Error(Module::Render, "Failed to create light buffer %d.", i);
            return false;
        }
        
        ret = _occluders[i].create();
        if(false == ret)
        {
            Log_Error(Module::Render, "Failed to create occluder stream %d.", i);
            return ret;
        }

        ret = _occluders[i].add(&_occludersVertexBuffer, 0, occluderAttributes[i]);
        if(false == ret)
        {
            Log_Error(Module::Render, "Failed to create occluder stream %d.", i);
            return ret;
        }
    }

    // [todo] do the same for spot light and directional light
    ret = ret && _occluders[LightType::POINT_LIGHT].add(&_buffer[LightType::POINT_LIGHT], 1, Geometry::ComponentType::FLOAT, 4, false, sizeof(float[8]),                0, 1);
    ret = ret && _occluders[LightType::POINT_LIGHT].add(&_buffer[LightType::POINT_LIGHT], 2, Geometry::ComponentType::FLOAT, 4, false, sizeof(float[8]), sizeof(float[4]), 1);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create occluder stream.");
        return ret;
    }

    _occluders[LightType::POINT_LIGHT].add(&_occludersIndexBuffer);
    _occluders[LightType::SPOT_LIGHT].add(&_occludersIndexBuffer);

    for(size_t i=0; i<LightType::COUNT; i++)
    {
        ret = _occluders[i].compile();
        if(false == ret)
        {
            Log_Error(Module::Render, "Failed to compile occluder stream %d.", i);
            return false;
       }
    }
    
    return true;
}

void LightPass::destroy()
{
    _gbuffer     = nullptr;
    _depthbuffer = nullptr;
    if(_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    _program.destroy();
    _output.destroy();
    
    _view.destroy();
    
    _occludersVertexBuffer.destroy();
    _occludersIndexBuffer.destroy();
    
    for(size_t i=0; i<LightType::COUNT; i++)
    {
        _occluders[i].destroy();
        _buffer[i].destroy();
    }
}

void LightPass::clear()
{
    for(size_t i=0; i<LightType::COUNT; i++)
    {
        _count[i] = 0;
    }
}

bool LightPass::add(PointLight const& light)
{
    if(_count[LightType::POINT_LIGHT] >= LightType::maxCount[LightType::POINT_LIGHT])
    {
        Log_Error(Module::Render, "Reached maximum number of point lights.");
        return false;
    }
    
    _buffer[LightType::POINT_LIGHT].bind();
    size_t elementSize = LightType::elementCount[LightType::POINT_LIGHT] * sizeof(float);
    float* ptr = (float*)_buffer[LightType::POINT_LIGHT].map(BufferObject::Access::Policy::WRITE_ONLY, elementSize * _count[LightType::POINT_LIGHT], elementSize);
    if(nullptr == ptr)
    {
        _buffer[LightType::POINT_LIGHT].unbind();
        Log_Error(Module::Render, "Failed to add point light.");
        return false;
    }
    ptr[0] = light.position.x;
    ptr[1] = light.position.y;
    ptr[2] = light.position.z;
    ptr[3] = light.radius;
    ptr[4] = light.color.x;
    ptr[5] = light.color.y;
    ptr[6] = light.color.z;
    ptr[7] = 0.0f;

    _buffer[LightType::POINT_LIGHT].unmap();
    _buffer[LightType::POINT_LIGHT].unbind();
    _count[LightType::POINT_LIGHT]++;
    return true;
}

void LightPass::draw(Camera const& camera)
{
    Renderer& renderer = Renderer::instance();

    glViewport(0, 0, _output.size().x, _output.size().y);
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderer.culling(false);
    renderer.depthBufferWrite(false);
    renderer.depthTest(true);
    renderer.blend(true);
    renderer.blendFunc(Framework::Render::BlendFunc::ONE, Framework::Render::BlendFunc::ONE);

    renderer.texture2D(true);
    renderer.setActiveTextureUnit(0);
    _gbuffer->bind();

    _program.begin();
        _view.bindTarget(1);
        float* ptr = (float*)_view.map(BufferObject::Access::Policy::WRITE_ONLY);
            glm::mat4 viewProjMatrix = camera.projectionMatrix(_output.size()) * camera.viewMatrix();
            memcpy(ptr,    glm::value_ptr(viewProjMatrix), sizeof(glm::mat4));
            memcpy(ptr+16, glm::value_ptr(camera.eye),     sizeof(glm::vec3));
        _view.unmap();
        _occluders[LightType::POINT_LIGHT].bind();
            glDrawElementsInstanced(GL_TRIANGLES, 12*3, GL_UNSIGNED_BYTE, 0, _count[LightType::POINT_LIGHT]);
        _occluders[LightType::POINT_LIGHT].unbind();
        _view.unbind();
    _program.end();

    renderer.setActiveTextureUnit(0);
    _gbuffer->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);

    renderer.depthBufferWrite(true);
}

Texture2D* LightPass::output()
{
    return &_output;
}

void LightPass::debug(glm::ivec2 const& pos, glm::ivec2 const& size)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, _output.size().x, _output.size().y, pos.x, pos.y, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(GL_FRONT);
}

} // Render
} // Framework
