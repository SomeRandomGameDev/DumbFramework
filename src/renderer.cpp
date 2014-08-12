#include <GL/glew.h>
#include <DumbFramework/log.hpp>
#include <DumbFramework/renderer.hpp>

namespace Framework {
    
Renderer::Renderer()
{}
Renderer::Renderer(Renderer const&)
{}
Renderer& Renderer::operator=(Renderer const&)
{ return *this; }
/**
* Destructor.
*/
Renderer::~Renderer()
{}
/**
* Get renderer unique instance.
* @return Reference to the renderer instance.
*/
Renderer& Renderer::instance()
{
    static Renderer renderer;
    return renderer;
}
/**
* Specify the texture unit to make active.
* All textures related operations will be bound to this unit.
* The texture bound to this unit will be accessible from the
* shaders provided you associated its unit number to one of
* the sampler uniforms.
* @param [in] unit  Texture unit number.
*/
void Renderer::activeTextureUnit(unsigned int unit)
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint maxTextureUnitCount;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint*)&maxTextureUnitCount);
    if(unit >= maxTextureUnitCount)
    {
        Log_Error(Module::Render, "Texture unit number out of bound %d (max %d).", unit, maxTextureUnitCount-1);
    }
#endif // SANITY_CHECK
    
    glActiveTexture(GL_TEXTURE0 + unit);
    
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLenum err= glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Module::Render, "An error occured while setting texture unit %d active: %s.", unit, gluErrorString(err));
    }
#endif
}
/**
* Retrieve the currently active texture unit.
* @return Active texture unit.
*/
unsigned int Renderer::currentActiveTextureUnit()
{
    unsigned int unit;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&unit);
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLenum err= glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Module::Render, "An error occured while retrieving currently active texture unit: %s.", unit, gluErrorString(err));
    }
#endif
    return unit;
}

/**
 * Enable or disable writing to the depth buffer.
 * @param [in] enable If true, writing to depth buffer is
 *                    enabled.
 */
void Renderer::depthBufferWrite(bool enable)
{
    glDepthMask(static_cast<GLboolean>(enable));
}

} // Framework
