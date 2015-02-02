#include <DumbFramework/render/light/lightpass.hpp>

namespace Framework {
namespace Render    {

 // [todo] remove 
static const GLfloat g_AABBVertices[] =
{
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f
};
 // [todo] remove 
static const uint8_t g_AABBIndices[] =
{
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
};

static const char* g_spotLightVertexShader = R"EOT(
#version 410 core
layout (location=0) in vec4 light_position;
layout (location=1) in vec4 light_color;
layout (location=2) in vec3 vs_position;
uniform mat4 viewProjMatrix;
out VS_OUT
{
    flat vec4  center;
    flat vec4  color;
} vs_out;
void main()
{
    vs_out.center = light_position;
    vs_out.color  = light_color;
    gl_Position   = viewProjMatrix * vec4(light_position.xyz + vs_position*light_position.w, 1.0);
}
)EOT";

static const char* g_spotLightFragmentShader = R"EOT(
#version 410 core
uniform sampler2DArray gbuffer;
in VS_OUT
{
    flat vec4  color;
    flat vec4  center;
} fs_in;
layout (location=0) out vec4 lightoutput;
void main()
{
    
    vec3 position = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 3), 0).xyz;
    if(distance(position, fs_in.center.xyz) > fs_in.center.w)
    {
        lightoutput = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 2), 0);
    }
    else
    {
        vec3 normal   = texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 2), 0).xyz;
        vec3 light    = normalize(fs_in.center.xyz - position);
        // [todo] BRDF
        lightoutput = vec4(texelFetch(gbuffer, ivec3(gl_FragCoord.xy, 0), 0).xyz * clamp(dot(normal, light), 0.0, 1.0), 1.0);
    }
}
)EOT";

#define MAX_LIGHT_COUNT 8

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
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer->id());
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
//                            {Render::Shader::Type::GEOMETRY_SHADER, g_spotLightGeometryShader},
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
        _viewProjMatrixId = _program.getUniformLocation("viewProjMatrix");
        _viewMatrixId     = _program.getUniformLocation("viewMatrix");
        id = _program.getUniformLocation("gbuffer");
        _program.uniform(id, 0);
    _program.end();

    // [todo] make MAX_LIGHT_COUNT a parameter
    ret = _vertexBuffer.create(MAX_LIGHT_COUNT * sizeof(float[8]), nullptr, Render::BufferObject::Access::Frequency::DYNAMIC, Render::BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create vertex buffer.");
        return false;
    }
    
    // [todo] Remove
    ret = _AABBVertexBuffer.create(8 * sizeof(float[3]), (void*)g_AABBVertices, Render::BufferObject::Access::Frequency::STATIC, Render::BufferObject::Access::Type::DRAW);
    ret = _AABBIndexBuffer.create(14 * sizeof(uint8_t), (void*)g_AABBIndices, Render::BufferObject::Access::Frequency::STATIC, Render::BufferObject::Access::Type::DRAW);
    
    // Create vertex stream
    _vertexStream.create();
    ret = _vertexStream.add(&_vertexBuffer,
                            {
                                { 0, Geometry::Attribute(Geometry::ComponentType::FLOAT, 4, false, sizeof(float[8]), 0               , 1)},
                                { 1, Geometry::Attribute(Geometry::ComponentType::FLOAT, 4, false, sizeof(float[8]), sizeof(float[4]), 1)}
                            });
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create vertex stream.");
        return false;
    }

    // [todo] Remove
    ret = _vertexStream.add(&_AABBVertexBuffer,
                            {{ 2, Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, sizeof(float[3]), 0, 0)}});
    ret = _vertexStream.add(&_AABBIndexBuffer);

    ret = _vertexStream.compile();
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to compile vertex stream.");
        return false;
    }

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

    return true;
}

void LightPass::draw(Camera const& camera)
{
    Renderer& renderer = Renderer::instance();
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glViewport(0, 0, _output.size().x, _output.size().y);
    glClearColor(0.0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderer.depthBufferWrite(false);
    renderer.setDepthFunc(DepthFunc::LESS_EQUAL);
    renderer.depthTest(true);
    renderer.culling(false);
    renderer.blend(true);
    renderer.blendFunc(Render::BlendFunc::SRC_ALPHA, Render::BlendFunc::ONE_MINUS_SRC_ALPHA);

    renderer.setActiveTextureUnit(0);
    _gbuffer->bind();
    renderer.texture2D(true);

    _program.begin();
    glm::mat4 viewProj = camera.projectionMatrix(_output.size()) * camera.viewMatrix();
    _program.uniform(_viewProjMatrixId, false, viewProj);
    _program.uniform(_viewMatrixId,     false, camera.viewMatrix());

    _vertexStream.bind();
        glDrawElementsInstanced(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_BYTE, 0, _count);
//        glDrawArrays(GL_POINTS, 0, _count);
    _vertexStream.unbind();

    renderer.setActiveTextureUnit(0);
    _gbuffer->unbind();

    _program.end();

    renderer.blend(false);

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
    glBlitFramebuffer(0, 0, _output.size().x, _output.size().y, pos.x, pos.y, size.x, size.y, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(GL_FRONT);
}

} // Render
} // Framework
