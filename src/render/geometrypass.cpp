#include <DumbFramework/render/geometrypass.hpp>
#include <DumbFramework/render/renderer.hpp>
#include <DumbFramework/log.hpp>

namespace Framework {
namespace Render    {

// tangent and bitangent are useless if the technique described in the
// following link is used (from three.js):
// http://mmikkelsen3d.blogspot.sk/2012/02/parallaxpoc-mapping-and-no-tangent.html
static const char* g_vertexShader = R"EOT(
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
)EOT";

static const char* g_fragmentShader = R"EOT(
#version 410 core
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
// [todo] more? sampleArray?
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
)EOT";

static const GLenum g_drawBuffers[GeometryPass::OUTPUT_LAYER_COUNT] = 
{
    GL_COLOR_ATTACHMENT0 + GeometryPass::ALBEDO,
    GL_COLOR_ATTACHMENT0 + GeometryPass::SPECULAR,
    GL_COLOR_ATTACHMENT0 + GeometryPass::NORMAL,
    GL_COLOR_ATTACHMENT0 + GeometryPass::WORLD_POS
};

/**
 * Default constructor.
 */
GeometryPass::GeometryPass()
{}
/**
 * Destructor.
 */
GeometryPass::~GeometryPass()
{}

/**
 * Create geometry pass.
 * This will create and initialize the output textures, the render
 * context and load the appropriate shaders.
 * @param [in] viewportSize Viewport Size.
 * @return false if an error occured, true otherwise.
 */
bool GeometryPass::create(glm::ivec2 const& viewportSize)
{
    bool ret;
    
    _viewportSize = viewportSize;

    ret = _output.create(viewportSize, Texture::PixelFormat::RGBA_32F, OUTPUT_LAYER_COUNT);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create geometry pass output texture.");
        return false;
    }
    _output.bind();
        _output.setMagFilter(Render::Texture::MagFilter::NEAREST_TEXEL);
        _output.setMinFilter(Render::Texture::MinFilter::NEAREST_TEXEL);
        _output.setWrap(Render::Texture::Wrap::CLAMP_TO_EDGE, Render::Texture::Wrap::CLAMP_TO_EDGE);
    _output.unbind();

    ret = _depthbuffer.create(viewportSize, Texture::PixelFormat::DEPTH_24);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create depth render buffer.");
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

    for(int i=0; i<OUTPUT_LAYER_COUNT; i++)
    {
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, _output.id(), 0, i);
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthbuffer.id());

    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE != status)
    {
        GLenum err = glGetError();
        Log_Error(Module::Render, "%s",(const char*)gluErrorString(err));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ret = _program.create( {{Render::Shader::Type::VERTEX_SHADER,   g_vertexShader},
                            {Render::Shader::Type::FRAGMENT_SHADER, g_fragmentShader}} );
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

    _view.create(sizeof(float[16]), nullptr, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create point view matrices buffer.");
        return false;
    }

    int id;
    _program.begin();
        _modelMatrixId    = _program.getUniformLocation("modelMatrix");
        _normalMatrixId   = _program.getUniformLocation("normalMatrix");
        id = _program.getUniformLocation("diffuseMap");
        _program.uniform(id, (int)Material::DIFFUSE);
        id = _program.getUniformLocation("specularMap");
        _program.uniform(id, (int)Material::SPECULAR);
        id = _program.getUniformLocation("normalMap");
        _program.uniform(id, (int)Material::NORMAL);
    _program.end();

    return true;
}

/**
 * Release the resources created by the current instance.
 */
void GeometryPass::destroy()
{
    if(_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    _program.destroy();
    _output.destroy();
    _depthbuffer.destroy();
    _view.destroy();
}

void GeometryPass::begin(Camera const& camera)
{
    Renderer& renderer = Renderer::instance();
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glDrawBuffers(GeometryPass::OUTPUT_LAYER_COUNT, g_drawBuffers);
    glViewport(0, 0, _viewportSize.x, _viewportSize.y);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderer.depthBufferWrite(true);
    renderer.setDepthFunc(DepthFunc::LESS);
    renderer.depthTest(true);

    _program.begin();
    _view.bindTarget(0);
        float* ptr = (float*)_view.map(BufferObject::Access::Policy::WRITE_ONLY, 0, sizeof(float[16]));
            glm::mat4 viewProj = camera.projectionMatrix(_viewportSize) * camera.viewMatrix();
            memcpy(ptr, glm::value_ptr(viewProj), sizeof(glm::mat4));
        _view.unmap();
}

void GeometryPass::end()
{
    _view.unbind();
    _program.end();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
}

/**
 * Render a single mesh.
 */
void GeometryPass::render(Material& material, glm::mat4 const& modelMatrix, glm::mat3 const& normalMatrix, Mesh const& mesh)
{
    material.bind();
        _program.uniform(_modelMatrixId, false, modelMatrix);
        _program.uniform(_normalMatrixId, false, normalMatrix);
        mesh.vertexStream().bind();
        glDrawElements(GL_TRIANGLES, mesh.triangleCount()*3, GL_UNSIGNED_INT, 0);
        mesh.vertexStream().unbind();
    material.unbind();
}

Texture2D* GeometryPass::output()
{
    return &_output;
}

Renderbuffer* GeometryPass::depthbuffer()
{
    return &_depthbuffer;
}

void GeometryPass::debug(OutputLayer layer, glm::ivec2 const& pos, glm::ivec2 const& size)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_COLOR_ATTACHMENT0+layer);
    glBlitFramebuffer(0, 0, _output.size().x, _output.size().y, pos.x, pos.y, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(GL_FRONT);
}


} // Render
} // Framework
