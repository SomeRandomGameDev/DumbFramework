#ifndef _DUMB_FW_RENDERER_
#define _DUMB_FW_RENDERER_

namespace Framework    {

/**
 * @defgroup DUMB_FW_RENDERING Graphical rendering.
 * Everything related to graphical rendering.
 */

/**
 * Depth tests comparaison function.
 * @ingroup DUMB_FW_RENDERING
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
    inline DepthFunc() : value(LESS) {}
    /**
     * Constructor.
     * @param [in] v Depth test comparaison function.
     */
    inline DepthFunc(Value v) : value(v) {}
    inline operator Value() { return value; }
};

/**
 * Blending functions.
 * @ingroup DUMB_FW_RENDERING
 */
struct BlendFunc
{
    /**
     * Blending function values.
     */
    enum Value
    {
        /**
         *  RGB Blend Factors | Alpha Blend Factors
         *  :---------------: | :-----------------:
         *  \f$(0,0,0)\f$     | \f$0\f$
         */
        ZERO,
        /**
         *  RGB Blend Factors | Alpha Blend Factors
         *  :---------------: | :-----------------:
         *  \f$(1,1,1)\f$     | \f$1\f$
         */
        ONE,
        /**
         *  RGB Blend Factors              | Alpha Blend Factors
         *  :----------------------------: | :-----------------:
         *  \f$(R_{s0},G_{s0},B_{s0})\f$   | \f$A_{s0}\f$
         */
        SRC_COLOR,
        /**
         *  RGB Blend Factors                      | Alpha Blend Factors
         *  :------------------------------------: | :-----------------:
         *  \f$(1,1,1)-(R_{s0},G_{s0},B_{s0})\f$   | \f$1-A_{s0}\f$
         */
        ONE_MINUS_SRC_COLOR,
        /**
         *  RGB Blend Factors          | Alpha Blend Factors
         *  :------------------------: | :-----------------:
         *  \f$(R_{d},G_{d},B_{d})\f$  | \f$A_{d}\f$
         */
        DST_COLOR,
        /**
         *  RGB Blend Factors                  | Alpha Blend Factors
         *  :--------------------------------: | :-----------------:
         *  \f$(1,1,1)-(R_{d},G_{d},B_{d})\f$  | \f$1-A_{d}\f$
         */
        ONE_MINUS_DST_COLOR,
        /**
         *  RGB Blend Factors              | Alpha Blend Factors
         *  :----------------------------: | :-----------------:
         *  \f$(A_{s0},A_{s0},A_{s0})\f$   | \f$A_{s0}\f$
         */
        SRC_ALPHA,
        /**
         *  RGB Blend Factors                      | Alpha Blend Factors
         *  :------------------------------------: | :-----------------:
         *  \f$(1,1,1)-(A_{s0},A_{s0},A_{s0})\f$   | \f$1-A_{s0}\f$
         */
        ONE_MINUS_SRC_ALPHA,
        /**
         *  RGB Blend Factors           | Alpha Blend Factors
         *  :-------------------------: | :-----------------:
         *  \f$(A_{d},A_{d},A_{d})\f$   | \f$A_{d}\f$
         */
        DST_ALPHA,
        /**
         *  RGB Blend Factors                  | Alpha Blend Factors
         *  :--------------------------------: | :-----------------:
         *  \f$(1,1,1)-(A_{d},A_{d},A_{d})\f$  | \f$1-A_{d}\f$
         */
        ONE_MINUS_DST_ALPHA,
        /**
         *  RGB Blend Factors           | Alpha Blend Factors
         *  :-------------------------: | :-----------------:
         *  \f$(R_{c},G_{c},B_{c})\f$   | \f$A_{c}\f$
         */
        CONSTANT_COLOR,
        /**
         *  RGB Blend Factors                  | Alpha Blend Factors
         *  :--------------------------------: | :-----------------:
         *  \f$(1,1,1)-(R_{c},G_{c},B_{c})\f$  | \f$1-A_{c}\f$
         */
        ONE_MINUS_CONSTANT_COLOR,
        /**
         *  RGB Blend Factors           | Alpha Blend Factors
         *  :-------------------------: | :-----------------:
         *  \f$(A_{c},A_{c},A_{c})\f$   | \f$A_{c}\f$
         */
        CONSTANT_ALPHA,
        /**
         *  RGB Blend Factors                  | Alpha Blend Factors
         *  :--------------------------------: | :-----------------:
         *  \f$(1,1,1)-(A_{c},A_{c},A_{c})\f$  | \f$1-A_{c}\f$
         */
        ONE_MINUS_CONSTANT_ALPHA,
        /**
         *  RGB Blend Factors  | Alpha Blend Factors
         *  :----------------: | :-----------------:
         *  \f$(f,f,f)\f$      | \f$1\f$
         * \f$f=min(A_{s0}, 1-A_{d})\f$
         */
        SRC_ALPHA_SATURATE
    };
    /** Current blending functions. **/
    Value value;
    /**
     * Default constructor.
     * By default the blending function is set to ONE.
     */
    inline BlendFunc() : value(BlendFunc::ONE) {}
    /**
     * Constructor.
     * @param [in] v Blending function.
     */
    inline BlendFunc(Value v) : value(v) {}
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
        
        /**
         * Set blending functions.
         * @param [in] src Source blending function.
         * @param [in] dst Destination blending function.
         */
        void blendFunc(BlendFunc src, BlendFunc dst);
        
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
