#include <GL/glew.h>

#include <DumbFramework/log.hpp>
#include <DumbFramework/render/renderer.hpp>

namespace Framework {
namespace Render    {

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
 * Enable or disable 2D texture mapping.
 * @param [in] enable If true enable 2D texture mapping.
 */
void Renderer::texture2D(bool enable)
{
    (enable ? glEnable : glDisable)(GL_TEXTURE_2D);
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
void Renderer::setDepthFunc(TestFunc test)
{
    glDepthFunc(test.to());
}
/**
 * Retrieve current depth test comparaison function.
 * @return Depth test comparaison function currently used.
 */
TestFunc Renderer::getDepthFunc()
{
    GLenum func;
    glGetIntegerv(GL_DEPTH_FUNC, (GLint*)&func);
    return TestFunc::from(func);
}
/**
 * Enable of disable depth testing.
 * @param [in] enable If true, enable depth testing is enabled.
 */
void Renderer::depthTest(bool enable)
{
    (enable ? glEnable : glDisable)(GL_DEPTH_TEST);
}
/**
 * Check if depth test is enabled.
 * @return true if depth testing is enabled.
 */
bool Renderer::isDepthTestEnabled()
{
    return (GL_TRUE == glIsEnabled(GL_DEPTH_TEST));
}

/**
 * Set stencil plane writemask.
 * @param [in] face Specify the front and/or back stencil state.
 * @param [in] mask Bit mask disabling or enabling writing to
 *                  stencil planes.
 */
void Renderer::setStencilMask(CullFace face, unsigned int mask)
{
    glStencilMaskSeparate(face.to(), mask);
}

/**
 * Enable or disable stencil test.
 * @param [in] enable If true, enable stencil test is enabled.
 */
void Renderer::stencilTest(bool enable)
{
    (enable ? glEnable : glDisable)(GL_STENCIL_TEST);
}

/**
 * Check if stencil test is enabled.
 * @return true if stencil test is enabled.
 */
bool Renderer::isStencilTestEnabled()
{
    return (GL_TRUE == glIsEnabled(GL_STENCIL_TEST));
}

/**
 * Enable or disable blending.
 * @param [in] enable If true, enable blending.
 */
void Renderer::blend(bool enable)
{
    (enable ? glEnable : glDisable)(GL_BLEND);
}
/**
 * Set blending functions.
 * @param [in] src Source blending function.
 * @param [in] dst Destination blending function.
 */
void Renderer::blendFunc(BlendFunc src, BlendFunc dst)
{
    glBlendFunc(src.to(), dst.to());
}

/**
 * Enable or disable facet culling.
 * @param [in] enable If true, cull facets based on their winding in window coordinates.
 */
void Renderer::culling(bool enable)
{
    (enable ? glEnable : glDisable)(GL_CULL_FACE);
}

/**
 * Check if facet culling is enabled.
 * @return true if facet culling is enabled.
 */
bool Renderer::isCullingEnabled()
{
    return (GL_TRUE == glIsEnabled(GL_CULL_FACE));
}

/**
 * Specify culling mode.
 * @param [in] mode Culling mode.
 */
void Renderer::cullingMode(CullFace mode)
{
    glCullFace(mode.to());
}

/**
 * Specify the orientation of front-facing polygons.
 * @param [in] clockwise If true, the polygon is considered to be
 *                       clockwise, otherwise it is counterclockwise.
 */
void Renderer::polygonWinding(bool clockwise)
{
    glFrontFace(clockwise ? GL_CW : GL_CCW);
}

/**
 * Discard fragments that are outside the scissor box.
 * @param [in] enable If true, discard fragments that are outside
 *                    the scissor box.
 */
void Renderer::scissorTest(bool enable)
{
    (enable ? glEnable : glDisable)(GL_SCISSOR_TEST);
}

/**
 * Define scissor box.
 * @param [in] pos  Lower left corner of the scissor box.
 * @param [in] size Size of the scissor box.
 */
void Renderer::scissor(glm::ivec2 const& pos, glm::ivec2 const& size)
{
    glScissor(pos.x, pos.y, size.x, size.y);
}

} // Render
} // Framework
