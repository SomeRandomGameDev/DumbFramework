#ifndef _DUMB_FW_RENDERER_
#define _DUMB_FW_RENDERER_

namespace Framework    {

/**
 * @defgroup DUMB_FW_RENDERING Graphical rendering.
 * Everything related to graphical rendering.
 */

/**
 * Rendering state manager.
 * @ingroup DUMB_FW_RENDERING
 */
class Renderer
{
    public:
        /**
         * Destructor.
         */
        ~Renderer();
        
        /**
         * Get renderer unique instance.
         * @return Reference to the renderer instance.
         */
        static Renderer& instance();
        
        /**
         * Specify the texture unit to make active.
         * All textures related operations will be bound to this unit.
         * The texture bound to this unit will be accessible from the
         * shaders provided you associated its unit number to one of
         * the sampler uniforms.
         * @param [in] unit  Texture unit number.
         */
        void activeTextureUnit(unsigned int unit);
        /**
         * Retrieve the currently active texture unit.
         * @return Active texture unit.
         */
        unsigned int currentActiveTextureUnit();
        
        /**
         * Enable or disable writing to the depth buffer.
         * @param [in] enable If true, writing to depth buffer is
         *                    enabled.
         */
        void depthBufferWrite(bool enable);
        
        /**
         * 
         */
    private:
        Renderer();
        Renderer(Renderer const&);
        Renderer& operator=(Renderer const&); 
};

} // Framework

#endif // _DUMB_FW_RENDERER_
