#include <DumbFramework/render/light/lightpass.hpp>
#include <DumbFramework/render/dummy.hpp>

namespace Framework {
namespace Render    {

static const float g_fsVertexData[] =
{
    -1.0f,-1.0f,
    -1.0f, 1.0f,
     1.0f,-1.0f,
     1.0f, 1.0f
};

static const char* g_spotLightVertexShader = R"EOT(
#version 410 core
layout (location = 0) in vec2 vs_position;
void main(void)
{
    gl_Position = vec4(vs_position, 0.0, 1.0);
}
)EOT";

// [todo] one per light type
static const char* g_spotLightFragmentShader = R"EOT(
#version 410 core
#define MAX_POINT_LIGHTS 128
#define PI 3.1415926535897932384626433832795
struct PointLight
{
    vec4 position;
    vec4 color;
};

layout (binding=0) uniform sampler2DArray gbuffer;
layout (std140, binding=1) uniform UPointLights
{
    PointLight pointLights[128]; // [todo]
};
uniform unsigned int pointLightCount;
uniform vec3 eye;

layout (location=0) out vec4 color_out;

float SchlickFresnel(float u)
{
    float m = max(1.0-u, 0.0);
    float m2 = m*m;
    return m2*m2*m;
}
float GTR2(float ndoth, float a)
{
    float a2 = a*a;
    float t  = ndoth*ndoth*(a2-1.0) + 1.0;
    return a2 / (PI * t*t);
}
float G1V(float v, float k)
{
    return 1.0/(v*(1.0-k) + k);
}
vec3 brdf(float dotNV, float dotNL, float dotNH, float dotLH, float dotVH, vec3 diffColor, vec4 specColor)
{        
// [todo] indirect env with spherical harmonics
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
    float fh = SchlickFresnel(dotVH);
    vec3  fresnel      = mix(specColor.rgb, vec3(1.0), fh);
    float visibility   = G1V(dotNL, sqrRoughness) * G1V(dotNV, sqrRoughness);
    float distribution = GTR2(dotNH, sqrRoughness*sqrRoughness);
    vec3 specular  = fresnel * visibility * distribution;

    return (specular + diffuse);
}
void main(void)
{
    vec3 accum = vec3(0.0);
    vec3 albedo   = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 0), 0).xyz;
    vec4 specular = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 1), 0);
    vec3 normal   = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 2), 0).xyz;
    vec3 position = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 3), 0).xyz;
    vec3 view     = normalize(eye - position);
    float dotNV = dot(view, normal);
    
    for(unsigned int i=0; i<pointLightCount; i++)
    {
        vec4  lightPosition = pointLights[i].position;
        float lightDistance = distance(position, lightPosition.xyz);
        if(lightDistance <= lightPosition.w)
        {
            vec3 light = normalize(lightPosition.xyz - position);
            vec3 halfL = normalize(light + view);
            float dotNL = dot(light, normal);
            float dotNH = dot(halfL, normal);
            float dotVH = dot(halfL, view);
            float dotLH = dot(light, halfL);
            
// [todo] Check if the applied clamping is correct.
            float attenuation   = min(1.0 / (lightDistance*lightDistance), 1.0);

            accum += clamp(brdf(dotNV, dotNL, dotNH, dotLH, dotVH, albedo, specular) * pointLights[i].color * dotNL * attenuation, 0.0, 1.0);
        }
    }
    color_out = vec4(accum, 1.0);
}
)EOT";

#define MAX_LIGHT_COUNT 128

/**
 * Default constructor.
 */
LightPass::LightPass()
    : _gbuffer(nullptr)
    , _depthbuffer(nullptr)
    , _program()
    , _framebuffer(0)
    , _output()
    , _viewlMatrixId(-1)
    , _fsQuad()
    , _fsQuadBuffer()
    , _buffer()
    , _count(0)
    , _countId(-1)
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

    ret = _fsQuadBuffer.create(4*sizeof(float[2]), (void*)g_fsVertexData, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create fullscreen quad buffer!");
        return false;
    }

    ret = _fsQuad.create();
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create fullscreen quad stream!");
        return false;
    }
    ret = _fsQuad.add(&_fsQuadBuffer, 0, Geometry::ComponentType::FLOAT, 2, sizeof(float[2]), 0, 0);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to set fullscreen quad attribute.");
        return false;
    }
    ret = _fsQuad.compile();
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to compile fullscreen quad stream.");
        return false;
    }
    
    ret = _buffer.create(128*sizeof(float[8]), nullptr, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create point light data buffer.");
        return false;
    }
    
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
    
    _program.begin();
        _countId = _program.getUniformLocation("pointLightCount");
        _viewlMatrixId = _program.getUniformLocation("eye");
    _program.end();
    
    return true;
}

void LightPass::destroy()
{
    _gbuffer     = nullptr;
    _depthbuffer = nullptr;
    _count = 0;
    _countId = -1;
    _viewlMatrixId = -1;
    if(_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    _program.destroy();
    _output.destroy();
    _fsQuad.destroy();
    _fsQuadBuffer.destroy();
    _buffer.destroy();
}

void LightPass::clear()
{
    _count = 0;
}

bool LightPass::add(PointLight const& light)
{
    _buffer.bindTarget(1);
    float* ptr = (float*)_buffer.map(BufferObject::Access::Policy::WRITE_ONLY, _count*sizeof(float[8]), sizeof(float[8]));
    bool ret = (nullptr != ptr);
    if(ret)
    {
        ptr[0] = light.position.x;
        ptr[1] = light.position.y;
        ptr[2] = light.position.z;
        ptr[3] = light.radius;
        ptr[4] = light.color.x;
        ptr[5] = light.color.y;
        ptr[6] = light.color.z;
        ptr[7] = 0.0f;
        _buffer.unmap();
        _count++;
    }
    else
    {
        Log_Error(Module::Render, "[todo]");
    }
    _buffer.unbind();
    return ret;
}

void LightPass::draw(Camera const& camera)
{
    Renderer& renderer = Renderer::instance();

    glViewport(0, 0, _output.size().x, _output.size().y);
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderer.depthBufferWrite(false);
    renderer.depthTest(false);
    renderer.culling(false);
    renderer.blend(false);

    renderer.texture2D(true);
    renderer.setActiveTextureUnit(0);
    _gbuffer->bind();

    _program.begin();
    _buffer.bindTarget(1);
    _program.uniform(_viewlMatrixId, camera.eye);
    _program.uniform(_countId, _count);
        _fsQuad.bind();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        _fsQuad.unbind();
    _buffer.unbind();
    _program.end();

    renderer.setActiveTextureUnit(0);
    _gbuffer->unbind();

    renderer.depthBufferWrite(true);
    renderer.depthTest(true);
    renderer.culling(true);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
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
