#include <DumbFramework/render/light/lightpass.hpp>

namespace Framework {
namespace Render    {

static const char* g_spotLightVertexShader = R"EOT(
#version 410 core
// x,y,z (position) w (radius)
layout (location=0) in vec4 vs_position;
// r,g,b (color) a (intensity)
layout (location=3) in vec4 vs_color;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewProjMatrix;
noperspective centroid out vec4 billboardCenter;
void main()
{
    vec4 center = viewProjMatrix * modelMatrix * vec4(vs_position.xyz, 1.0);
    billboardCenter = vec4(center.xyz, vs_position.w*vs_position.w);
    vec3 delta;
    delta.z  = (billboardCenter.z*billboardCenter.z) - billboardCenter.w;
    delta.xy = (vs_position.w * sqrt((center.xy * center.xy) + delta.z) / delta.z;
    float focale = viewProjMatrix[0][0];
    float aspect = viewProjMatrix[1][1] / viewProjMatrix[0][0];
    center.xy  = -(center.z * focale * center.xy) / delta.z;
    center.y *= aspect;
    center.z  = aspect;
    gl_PointSize = focale * max(delta.x, delta.y);
    gl_Position = center
}
)EOT";
static const char* g_spotLightGeometryShader = R"EOT(
// [TODO]
)EOT";
static const char* g_spotLightFragmentShader = R"EOT(
// [TODO]
)EOT";

/**
 * Default constructor.
 */
LightPass::LightPass()
    : _gbuffer(nullptr)
    , _depthbuffer(nullptr)
    , _framebuffer(0)
    , _output()
    , _count(0)
    , _vertexBuffer()
    , _vertexStream()
    , _program()
{}
/**
 * Destructor.
 */
LightPass::~LightPass()
{
    destroy();
}

bool LightPass::create(Texture2D* gbuffer, Renderbuffer* depthbuffer)
{
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

    glGenFramebuffers(1, &_framebuffer);
    GLenum err = glGetError(); 
    if(GL_NO_ERROR != err)
    {
        Log_Error(Module::Render, (const char*)gluErrorString(err));
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _output.id(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer->id());

    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE != status)
    {
        GLenum err = glGetError();
        Log_Error(Module::Render, "%s",(const char*)gluErrorString(err));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // [todo] program creation
    // [todo] vertex buffer and stream creation

    return true;
}

void LightPass::destroy()
{
    _gbuffer = nullptr;
    _depthbuffer = nullptr;
    _count = 0;
    if(_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    _program.destroy();
    _output.destroy();
    _vertexBuffer.destroy();
    _vertexStream.destroy();;
}

void LightPass::clear()
{
    _count = 0;
}

bool LightPass::add(PointLight const& light)
{
    // [todo] add pointlight to vertexbuffer
    return true;
}

void LightPass::draw(Camera const& camera)
{
    // [todo] enable depth test
    // [todo] disable depth write
    // [todo] bind framebuffer
    // [todo] bind gbuffer
    // [todo] enable program
    // [todo] draw points
    // [todo] restore bindings/renderer
}

Texture2D* LightPass::output()
{
    return &_output;
}

} // Render
} // Framework
