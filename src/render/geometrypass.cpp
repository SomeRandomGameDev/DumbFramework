#include <DumbFramework/render/geometrypass.hpp>
#include <DumbFramework/render/renderer.hpp>
#include <DumbFramework/log.hpp>

namespace Framework {
namespace Render    {

static const char* g_vertexShader = R"EOT(
#version 410 core

layout (location=0) in vec3 vs_position;
layout (location=1) in vec2 vs_uv;
layout (location=2) in vec3 vs_normal;
// [todo] layout (location=3) in vec3 vs_tangent;
// [todo] layout (location=4) in vec3 vs_bitangent;
// [todo] layout (location=5) in mat4 vs_model;

uniform mat4 viewProj;

out vec3 worldPos;
out vec2 texCoord;
out vec3 normal;
// [todo] out vec3 tangent;
// [todo] out vec3 bitangent;

void main()
{
// [todo]    vec4 tmp = vs_model * vec4(vs_position, 1.0);
    vec4 tmp = vec4(vs_position, 1.0);
    gl_Position = viewProj *tmp;
    worldPos    = tmp.xyz;
    texCoord    = vs_uv;
    normal      = vs_normal;
// [todo]     normal      = (vs_model * vec4(vs_normal,    0.0)).xyz;
// [todo]     tangent     = (vs_model * vec4(vs_tangent,   0.0)).xyz;
// [todo]     bitangent   = (vs_model * vec4(vs_bitangent, 0.0)).xyz;
}
)EOT";

static const char* g_fragmentShader = R"EOT(
#version 410 core

// [todo] uniform Material {
    uniform sampler2D diffuseMap;
    uniform sampler2D specularMap;
// [todo] uniform sampler2D normalMap;
//  [...]
// [todo] };

in vec3 worldPos;
in vec2 texCoord;
in vec3 normal;
// [todo] in vec3 tangent;
// [todo] in vec3 bitangent;

layout (location=0) out vec4 albedo;
layout (location=1) out vec4 specular;
layout (location=2) out vec4 normalDepth;

void main()
{
    albedo      = texture(diffuseMap, texCoord);
    specular    = texture(specularMap, texCoord);
    normalDepth = vec4(normal, worldPos.z);
}
)EOT";

static const GLenum g_drawBuffers[GeometryPass::OUTPUT_LAYER_COUNT] = 
{
    GL_COLOR_ATTACHMENT0 + GeometryPass::ALBEDO,
    GL_COLOR_ATTACHMENT0 + GeometryPass::SPECULAR,
    GL_COLOR_ATTACHMENT0 + GeometryPass::NORMAL_DEPTH
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

    int id;
    _program.begin();
        _viewProjId = _program.getUniformLocation("viewProj");
        id = _program.getUniformLocation("diffuseMap");
        _program.uniform(id, (int)Material::DIFFUSE);
        id = _program.getUniformLocation("specularMap");
        _program.uniform(id, (int)Material::SPECULAR);
// [todo] normal Map etc...
    _program.end();

    return true;
}

/**
 * Release the resources created by the current instance.
 */
void GeometryPass::destroy()
{
    _program.destroy();
    _output.destroy();
}

void GeometryPass::begin()
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
}

void GeometryPass::end()
{
    _program.end();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
}

/**
 * Render a single mesh.
 */
void GeometryPass::render(Camera const& camera, Material& material, Mesh const& mesh)
{
    material.bind();
    glm::mat4 viewProj = camera.projectionMatrix(_viewportSize) * camera.viewMatrix();
        _program.uniform(_viewProjId, false, viewProj);

        mesh.vertexBuffer().bind();

        // [todo] make loop
        glEnableVertexAttribArray(0);
        mesh.attribute(Render::Mesh::Position).attach(0);

        glEnableVertexAttribArray(1);
        mesh.attribute(Render::Mesh::TexCoord).attach(1);

        glEnableVertexAttribArray(2);
        mesh.attribute(Render::Mesh::Normal).attach(2);

        // [todo] model matrix

        mesh.indexBuffer().bind();
        glDrawElements(GL_TRIANGLES, mesh.triangleCount()*3, GL_UNSIGNED_INT, 0);
        mesh.indexBuffer().unbind();

        mesh.vertexBuffer().unbind();
        
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

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
    glBlitFramebuffer(0, 0, _output.size().x, _output.size().y, pos.x, pos.y, size.x, size.y, GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(GL_FRONT);
}


} // Render
} // Framework
