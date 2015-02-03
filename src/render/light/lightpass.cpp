#include <DumbFramework/render/light/lightpass.hpp>
#include <DumbFramework/render/dummy.hpp>

namespace Framework {
namespace Render    {

static const char* g_spotLightVertexShader = R"EOT(
#version 410 core
void main(void)
{
    const vec2 vertices[4] = vec2[4]( vec2(-1.0,-1.0),
                                      vec2(-1.0, 1.0),
                                      vec2( 1.0,-1.0),
                                      vec2( 1.0, 1.0) );
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}
)EOT";

static const char* g_spotLightFragmentShader = R"EOT(
#version 410 core
uniform sampler2DArray gbuffer;
layout (location = 0) out vec4 color_out;
void main(void)
{
    // [todo] Create uniform buffer
    vec4 lightPosition = vec4(1.2, 0.0, 0.0, 0.75);
    
    vec3 accum = vec3(0.0);
    vec3 position = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 3), 0).xyz;
    if(distance(position, lightPosition.xyz) > lightPosition.w)
    {
        vec3 normal = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 2), 0).xyz;
        vec3 light = normalize(lightPosition.xyz - position);
        // [todo] BRDF
        accum += vec4(texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 0), 0).xyz * clamp(dot(normal, light), 0.0, 1.0), 1.0);
    }
    color_out = vec4(accum, 1.0);
}
)EOT";

#define MAX_LIGHT_COUNT 8

/**
 * Default constructor.
 */
LightPass::LightPass()
    : _gbuffer(nullptr)
    , _depthbuffer(nullptr)
    , _program()
    , _framebuffer(0)
    , _output()
    , _emptyVao(0)
    , _buffer()
    , _count(0)
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

    int id;
    _program.begin();
        id = _program.getUniformLocation("gbuffer");
        _program.uniform(id, 0);
    _program.end();

    glGenVertexArrays(1, &_emptyVao);

    return true;
}

void LightPass::destroy()
{
    _gbuffer     = nullptr;
    _depthbuffer = nullptr;
    _count = 0;
    if(_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    _program.destroy();
    _output.destroy();
    if(_emptyVao)
    {
        glDeleteVertexArrays(1, &_emptyVao);
        _emptyVao = 0;
    }
}

void LightPass::clear()
{
    _count = 0;
}

bool LightPass::add(PointLight const& light)
{
/*
    float* ptr = (float*)_vertexBuffer.map(BufferObject::Access::Policy::WRITE_ONLY, _count*sizeof(float[8]), sizeof(float[8]));
    if(nullptr == ptr)
    {
        Log_Error(Module::Render, "[todo]");
        return false;
    }

    ptr[0] = light.position.x;
    ptr[1] = light.position.y;
    ptr[2] = light.position.z;
    ptr[3] = light.radius;

    ptr[4] = light.color.x;
    ptr[5] = light.color.y;
    ptr[6] = light.color.z;
    ptr[7] = light.intensity;

    _vertexBuffer.unmap();

    _count++;
*/
    return true;
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

    glBindVertexArray(_emptyVao);
        _program.begin();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
        _program.end();
    glBindVertexArray(0);

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
