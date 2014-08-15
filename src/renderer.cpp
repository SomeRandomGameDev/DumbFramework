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
void Renderer::setActiveTextureUnit(unsigned int unit)
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
unsigned int Renderer::getActiveTextureUnit()
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
    return (unit - GL_TEXTURE0);
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

/**
 * Check if depth buffer writing is enabled.
 * @return true is writing to the depth buffer is enabled.
 */
bool Renderer::isDepthBufferWriteEnabled()
{
    GLboolean write;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &write);
    return (GL_TRUE == write); 
}

/**
 * Set depth test comparaison function.
 * @param [in] test Depth test comparaison function.
 */
void Renderer::setDepthFunc(DepthFunc test)
{
    GLenum func;
    
    switch(test.value)
    {
        case DepthFunc::NEVER:
            func = GL_NEVER;
            break;
        case DepthFunc::LESS:
            func = GL_LESS;
            break;
        case DepthFunc::EQUAL:
            func = GL_EQUAL;
            break;
        case DepthFunc::LESS_EQUAL: 
            func = GL_LEQUAL;
            break;
        case DepthFunc::GREATER:
            func = GL_GREATER;
            break;
        case DepthFunc::NOT_EQUAL:
            func = GL_NOTEQUAL;
            break;
        case DepthFunc::GREATER_EQUAL:
            func = GL_GEQUAL;
            break;
        case DepthFunc::ALWAYS:
            func = GL_ALWAYS;
            break;
    }
    
    glDepthFunc(func);
}

/**
 * Retrieve current depth test comparaison function.
 * @return Depth test comparaison function currently used.
 */
DepthFunc Renderer::getDepthFunc()
{
    GLenum func;
    
    glGetIntegerv(GL_DEPTH_FUNC, (GLint*)&func);
    
    switch(func)
    {
        case GL_NEVER:
            return DepthFunc::NEVER;
        case GL_LESS:
            return DepthFunc::LESS;
        case GL_EQUAL:
            return DepthFunc::EQUAL;
        case GL_LEQUAL:
            return DepthFunc::LESS_EQUAL;
        case GL_GREATER:
            return DepthFunc::GREATER;
        case GL_NOTEQUAL:
            return DepthFunc::NOT_EQUAL;
        case GL_GEQUAL:
            return DepthFunc::GREATER_EQUAL;
        case GL_ALWAYS:
            return DepthFunc::ALWAYS;
    }
    return DepthFunc::ALWAYS;
}

/**
 * Enable of disable depth testing.
 * @param [in] enable If true, enable depth testing is enabled.
 */
void Renderer::depthTest(bool enable)
{
    if(enable)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

/**
 * Check if depth test is enabled.
 * @return true if depth testing is enabled.
 */
bool Renderer::isDepthTestEnabled()
{
    return (GL_TRUE == glIsEnabled(GL_DEPTH_TEST));
}

} // Framework
