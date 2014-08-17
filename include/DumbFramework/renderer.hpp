#ifndef _DUMB_FW_RENDERER_
#define _DUMB_FW_RENDERER_

namespace Framework    {

/**
 * @defgroup DUMB_FW_RENDERING Graphical rendering.
 * Everything related to graphical rendering.
 */

/**
 * Depth tests comparaison function.
 */
struct DepthFunc
{
    /**
     * Depth test comparaison function values.
     */
    enum Value
    {
        /**
         * Depth test never passes. 
         */
        NEVER,
        /**
         * Depth test passes if the new depth value is less than the 
         * stored value.
         */
        LESS,
        /**
         * Depth test passes if the new depth value is equal to the
         * stored value.
         */
        EQUAL,
        /**
         * Depth test passes if the new depth value is less than or 
         * equal to the stored value.
         */
        LESS_EQUAL,
        /**
         * Depth test passes if the new depth value is greather than the 
         * stored value.
         */
        GREATER,
        /**
         * Depth test passes if the new depth value is not equal to the 
         * stored value.
         */
        NOT_EQUAL,
        /**
         * Depth test passes if the new depth value is greather than 
         * or equal to the stored value.
         */
        GREATER_EQUAL,
        /**
         * Depth test always passes.
         */
        ALWAYS 
    };
    /** Current depth test comparaison function. **/
    Value value;
    /**
     * Default constructor.
     * By default the depth test comparaison function is set to LESS.
     */
    DepthFunc() : value(LESS) {}
    /**
     * Constructor.
     * @param [in] v Depth test comparaison function.
     */
    DepthFunc(Value v) : value(v) {}
    inline operator Value() { return value; }
};

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
        void setActiveTextureUnit(unsigned int unit);
        /**
         * Retrieve the currently active texture unit.
         * @return Active texture unit.
         */
        unsigned int getActiveTextureUnit();
        
        /**
         * Enable or disable writing to the depth buffer.
         * @param [in] enable If true, writing to depth buffer is
         *                    enabled.
         */
        void depthBufferWrite(bool enable);
        
        /**
         * Check if depth buffer writing is enabled.
         * @return true is writing to the depth buffer is enabled.
         */
        bool isDepthBufferWriteEnabled();
        
        /**
         * Set depth test comparaison function.
         * @param [in] test Depth test comparaison function.
         */
        void setDepthFunc(DepthFunc test);
        
        /**
         * Retrieve current depth test comparaison function.
         * @return Depth test comparaison function currently used.
         */
        DepthFunc getDepthFunc();
        
        /**
         * Enable of disable depth testing.
         * @param [in] enable If true, enable depth testing is enabled.
         */
        void depthTest(bool enable);
        
        /**
         * Check if depth test is enabled.
         * @return true if depth testing is enabled.
         */
        bool isDepthTestEnabled();
        
        // depth range
        // blend mode
        // clear
        // more ...
        
        // cache values?
        
    private:
        Renderer();
        Renderer(Renderer const&);
        Renderer& operator=(Renderer const&); 
};

} // Framework

#endif // _DUMB_FW_RENDERER_
