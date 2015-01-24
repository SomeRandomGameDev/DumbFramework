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
    , _name(g_defaultMaterialName)
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
    renderer.blendFunc(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_SRC_ALPHA); // [todo]
    
    renderer.culling(culling);
    renderer.cullingMode(cullingMode);
    
    renderer.depthBufferWrite(depthWrite);
    renderer.depthTest(depthTest);
    renderer.setDepthFunc(depthFunc);
    
    renderer.texture2D(true);
    renderer.setActiveTextureUnit(DIFFUSE);
    diffuseMap.bind();
    renderer.setActiveTextureUnit(SPECULAR);
    specularMap.bind();
}

void Material::unbind()
{
    Renderer& renderer = Renderer::instance();

    renderer.setActiveTextureUnit(SPECULAR);
    specularMap.unbind();
    renderer.setActiveTextureUnit(DIFFUSE);
    diffuseMap.unbind();

    renderer.setActiveTextureUnit(0);
}

std::string const& Material::name() const
{
    return _name;
}

} // Render
} // Framework
