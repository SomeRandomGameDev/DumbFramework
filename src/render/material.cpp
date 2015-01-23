#include <DumbFramework/render/material.hpp>

namespace Framework {
namespace Render    {

static const char*  g_defaultMaterialName = "uninitialized";

Material::Material()
    : blend(false)
    , srcBLend(BlendFunc::SRC_COLOR)
    , dstBlend(BlendFunc::DST_COLOR)
    , depthWrite(true)
    , depthTest(true)
    , depthFunc(DepthFunc::LESS)
    , culling(true)
    , cullingMode(CullFace::BACK)
    , shininess(1.0f)
    , _name(g_defaultMaterialName)
    , _program()
{}

Material::~Material()
{
    destroy();
}

bool Material::create(std::string const& name)
{
    destroy();
    _name = name;
    
    blend = false;
    
    culling     = true;
    cullingMode = CullFace::BACK;
    
    depthWrite = true;
    depthTest  = true;
    depthFunc  = DepthFunc::LESS;
    
    shininess = 1.0f;
    
    // [todo]
    return true;
}

void Material::destroy()
{
    _name = g_defaultMaterialName;
    diffuseMap.destroy();
    specularMap.destroy();
}

void Material::bind()
{
    Renderer& renderer = Renderer::instance();

    renderer.blend(blend);
    renderer.blendFunc(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_SRC_ALPHA);
    
    renderer.culling(culling);
    renderer.cullingMode(cullingMode);
    
    renderer.depthBufferWrite(depthWrite);
    renderer.depthTest(depthTest);
    renderer.setDepthFunc(depthFunc);
    
    renderer.setActiveTextureUnit(0);
    diffuseMap.bind();
    renderer.setActiveTextureUnit(1);
    specularMap.bind();

    _program.begin();
    _program.uniform(_shininessId, shininess);
}

void Material::unbind()
{
    Renderer& renderer = Renderer::instance();

    _program.end();

    renderer.setActiveTextureUnit(1);
    specularMap.unbind();
    renderer.setActiveTextureUnit(0);
    diffuseMap.unbind();
}

std::string const& Material::name() const
{
    return _name;
}

void Material::attach(Program const& prog)
{
    _program = prog;
    _shininessId = _program.getUniformLocation("shininess");

    int diffuseId  = _program.getUniformLocation("diffuseMap");
    int specularId = _program.getUniformLocation("specularMap");

    _program.begin();
        _program.uniform(diffuseId,  0);
        _program.uniform(specularId, 1);
    _program.end();
}

} // Render
} // Framework
