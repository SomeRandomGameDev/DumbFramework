#include <DumbFramework/render/lightpass.hpp>
#include <DumbFramework/file.hpp>
#include "light/occluders.hpp"

namespace Framework {
namespace Render    {

/**
 * Default constructor.
 */
LightPass::LightPass()
    : _gbuffer(nullptr)
    , _depthbuffer(nullptr)
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

    // [todo] Don't forget to make a lovely loop?
    ret = createProgram(LightType::POINT_LIGHT, "/resources/shaders/pointlight.vs", "/resources/shaders/pointlight.fs");
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create point light shader.");
        return false;
    }

    ret = createProgram(LightType::SPOT_LIGHT, "/resources/shaders/spotlight.vs", "/resources/shaders/spotlight.fs");
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create point light shader.");
        return false;
    }

    ret = _view.create(2 * sizeof(float[16]), nullptr, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create point view matrices buffer.");
        return false;
    }

    clear();

    return true;
}

bool LightPass::createProgram(LightType type, std::string const& vertexShaderFilename, std::string const& fragmentShaderFilename)
{
    std::string shaderData[2];
    std::string filename[2] =
    {
        Framework::File::executableDirectory() + vertexShaderFilename,
        Framework::File::executableDirectory() + fragmentShaderFilename
    };

    bool ret;
    for(size_t i=0; i<2; i++)
    {
        File input;
        size_t nRead;

        ret = input.open(filename[i], File::READ_ONLY);
        if(false == ret)
        {
            Log_Error(Module::Render, "Failed to open shader %s", filename[i].c_str());
            return false;
        }
        shaderData[i].resize(input.size());
        nRead = input.read(&shaderData[i][0], input.size());
        input.close();
        if(nRead != shaderData[i].size())
        {
            Log_Error(Module::Render, "Failed to read shader %s", filename[i].c_str());
            return false;
        }
    }

    ret = _program[type].create( {{Render::Shader::Type::VERTEX_SHADER,   shaderData[0].c_str() },
                                  {Render::Shader::Type::FRAGMENT_SHADER, shaderData[1].c_str()}} );
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create program.");
        return false;
    }

    ret = _program[type].link();
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to link program.");
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
    _output.destroy();
    
    _view.destroy();
    
    _occludersVertexBuffer.destroy();
    _occludersIndexBuffer.destroy();
    
    for(size_t i=0; i<LightType::COUNT; i++)
    {
        _program[i].destroy();
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

    ptr[0] = light.color.x;
    ptr[1] = light.color.y;
    ptr[2] = light.color.z;
    ptr[3] = 0.0f;
    ptr[4] = light.position.x;
    ptr[5] = light.position.y;
    ptr[6] = light.position.z;
    ptr[7] = light.radius;

    _buffer[LightType::POINT_LIGHT].unmap();
    _buffer[LightType::POINT_LIGHT].unbind();
    _count[LightType::POINT_LIGHT]++;
    return true;
}

bool LightPass::add(SpotLight const& light)
{
    if(_count[LightType::SPOT_LIGHT] >= LightType::maxCount[LightType::SPOT_LIGHT])
    {
        Log_Error(Module::Render, "Reached maximum number of spot lights.");
        return false;
    }
    
    _buffer[LightType::SPOT_LIGHT].bind();
    size_t elementSize = LightType::elementCount[LightType::SPOT_LIGHT] * sizeof(float);
    float* ptr = (float*)_buffer[LightType::SPOT_LIGHT].map(BufferObject::Access::Policy::WRITE_ONLY, elementSize * _count[LightType::SPOT_LIGHT], elementSize);
    if(nullptr == ptr)
    {
        _buffer[LightType::SPOT_LIGHT].unbind();
        Log_Error(Module::Render, "Failed to add spot light.");
        return false;
    }

    ptr[ 0] = light.color.x;
    ptr[ 1] = light.color.y;
    ptr[ 2] = light.color.z;
    ptr[ 3] = light.radius;
    ptr[ 4] = light.position.x;
    ptr[ 5] = light.position.y;
    ptr[ 6] = light.position.z;
    ptr[ 7] = light.innerConeAngle;
    ptr[ 8] = light.direction.x;
    ptr[ 9] = light.direction.y;
    ptr[10] = light.direction.z;
    ptr[11] = light.outerConeAngle;

    _buffer[LightType::SPOT_LIGHT].unmap();
    _buffer[LightType::SPOT_LIGHT].unbind();
    _count[LightType::SPOT_LIGHT]++;
    return true;
}

void LightPass::draw(Dumb::Core::Geometry::Camera const& camera)
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

    for(size_t i=0; i<LightType::COUNT; i++)
    {
        if(_count[i])
        {
            _program[i].begin();
                _view.bindTarget(1);
                float* ptr = (float*)_view.map(BufferObject::Access::Policy::WRITE_ONLY);
                    glm::mat4 viewProjMatrix = camera.projectionMatrix(_output.size()) * camera.viewMatrix();
                    memcpy(ptr,    glm::value_ptr(viewProjMatrix), sizeof(glm::mat4));
                    memcpy(ptr+16, glm::value_ptr(camera.eye),     sizeof(glm::vec3));
                _view.unmap();
                _occluders[i].bind();
                    Light::drawOccluders[i](_count[i]);//glDrawElementsInstanced(GL_TRIANGLES, 12*3, GL_UNSIGNED_BYTE, 0, _count[i]);
                _occluders[i].unbind();
                _view.unbind();
            _program[i].end();
        }
    }

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
