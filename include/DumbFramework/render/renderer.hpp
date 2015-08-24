/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _DUMB_FW_RENDERER_
#define _DUMB_FW_RENDERER_

#include <glm/glm.hpp>

namespace Dumb   {
namespace Render {

/**
 * @defgroup DUMB_FW_RENDERING Graphical rendering.
 * Everything related to graphical rendering.
 */

/**
 * Test function.
 * Used in depth and stencil tests.
 * @ingroup DUMB_FW_RENDERING
 */
struct TestFunc
{
    /**
     * Test function values.
     */
    enum Value
    {
        /**
         * Test never passes. 
         */
        NEVER,
        /**
         * Test passes if the new value is less than the stored value.
         */
        LESS,
        /**
         * Test passes if the new value is equal to the stored value.
         */
        EQUAL,
        /**
         * Test passes if the new value is less than or equal to the
         * stored value.
         */
        LESS_EQUAL,
        /**
         * Test passes if the new value is greather than the stored
         * value.
         */
        GREATER,
        /**
         * Test passes if the new value is not equal to the stored
         * value.
         */
        NOT_EQUAL,
        /**
         * Test passes if the new value is greather than or equal to the
         * stored value.
         */
        GREATER_EQUAL,
        /**
         * Test always passes.
         */
        ALWAYS 
    };
    /** Current test function. **/
    Value value;
    /**
     * Default constructor.
     * By default the test function is set to LESS.
     */
    inline TestFunc() : value(LESS) {}
    /**
     * Constructor.
     * @param [in] v Test function.
     */
    inline TestFunc(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** Convert to OpenGL compliant value. **/
    inline GLenum to() const;
    /** Create from OpenGL value. **/
    inline static TestFunc from(GLenum test);
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
    /** Convert to OpenGL compliant value. **/
    inline GLenum to() const;
    /** Create from OpenGL value. **/
    inline static BlendFunc from(GLenum mode);
};

/**
 * Face culling modes. 
 * @ingroup DUMB_FW_RENDERING
 */
struct CullFace
{
    /**
     * Facet culling mode values.
     */
    enum Value
    {
        /**
         * Front facing facets are culled.
         */
        FRONT,
        /**
         * Back facing facets are culled.
         */
        BACK,
        /**
         * No facets are drawn but points and lines are drawn.
         */
        FRONT_AND_BACK,
    };
    /**  **/
    Value value;
    /**
     * Default constructor.
     */
    inline CullFace() : value(BACK) {}
    /**
     * Constructor.
     * @param [in] v 
     */
    inline CullFace(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** Convert to OpenGL compliant value. **/
    inline GLenum to() const;
    /** Create from OpenGL value. **/
    inline static CullFace from(GLenum test);
};

/**
 * Value operation.
 * @ingroup DUMB_FW_RENDERING
 */
struct Operation
{
    /**
     *Operation values.
     */
    enum Value
    {
        /**
         * Keep current value.
         */
        KEEP,
        /**
         * Set value to 0.
         */
        ZERO,
        /**
         * Replace value.
         */
        REPLACE,
        /**
         * Increments current value and clamp to the maximum unsigned value.
         */
        INCREMENT,
        /**
         * Increments current value and wraps to zero on overflow.
         */
        INCREMENT_WRAP,
        /**
         * Decrements current value and clamp to zero.
         */
        DECREMENT,
        /**
         * decrements current value and wraps to the maximum unsigned value on overflow.
         */
        DECREMENT_WRAP,
        /**
         * Bitwise inverts current value.
         */
        INVERT
    };
    /**  **/
    Value value;
    /**
     * Default constructor.
     */
    inline Operation() : value(KEEP) {}
    /**
     * Constructor.
     * @param [in] v 
     */
    inline Operation(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** Convert to OpenGL compliant value. **/
    inline GLenum to() const;
    /** Create from OpenGL value. **/
    inline static Operation from(GLenum test);
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
         * Enable or disable 2D texture mapping.
         * @param [in] enable If true enable 2D texture mapping.
         */
        void texture2D(bool enable);
        
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
        void setDepthFunc(TestFunc test);
        
        /**
         * Retrieve current depth test comparaison function.
         * @return Depth test comparaison function currently used.
         */
        TestFunc getDepthFunc();
        
        /**
         * Enable or disable depth testing.
         * @param [in] enable If true, enable depth testing is enabled.
         */
        void depthTest(bool enable);
        
        /**
         * Check if depth test is enabled.
         * @return true if depth testing is enabled.
         */
        bool isDepthTestEnabled();
        
        /**
         * Set stencil plane writemask.
         * @param [in] face Specify the front and/or back stencil state.
         * @param [in] mask Bit mask disabling or enabling writing to
         *                  stencil planes.
         */
        void setStencilMask(CullFace face, unsigned int mask);
        
        /**
         * Enable or disable stencil test.
         * @param [in] enable If true, enable stencil test is enabled.
         */
        void stencilTest(bool enable);
        
        /**
         * Set stencil function and reference value.
         * @param [in] face      Specify the front and/or back stencil state.
         * @param [in] function  Test function.
         * @param [in] ref       Reference value for test.
         * @param [in] mask      Mask value.
         * Test function | Result
         * --------------| -----------------------------------------------
         * NEVER         | Always fails. 
         * LESS          | Passes if ( ref & mask ) <  ( stencil & mask )
         * LESS_EQUAL    | Passes if ( ref & mask ) <= ( stencil & mask )
         * GREATER       | Passes if ( ref & mask ) >  ( stencil & mask )
         * GREATER_EQUAL | Passes if ( ref & mask ) >= ( stencil & mask )
         * EQUAL         | Passes if ( ref & mask ) =  ( stencil & mask )
         * NOT_EQUAL     | Passes if ( ref & mask ) != ( stencil & mask )
         * ALWAYS        | Always passes. 
         */
        void stencilFunc(CullFace face, TestFunc function, int ref, unsigned int mask);
        
        /**
         * Specify the ooperation performed on the associated value for
         * a given test.
         * @param [in] face         Specify the front and/or back stencil state.
         * @param [in] stencilFail  Action performed when the stencil test fails.
         * @param [in] depthFail    Action performed when the depth test fails.
         * @param [in] depthPass    Action performed when both the depth
         *                          and stencil test pass, or when there is
         *                          no depth buffer attached or depth test
         *                          is disabled and stencil test passes.
         */
        void stencilOp(CullFace face, Operation stencilFail, Operation depthFail, Operation depthPass);
        
        /**
         * Check if stencil test is enabled.
         * @return true if stencil test is enabled.
         */
        bool isStencilTestEnabled();
        
        /**
         * Enable or disable blending.
         * @param [in] enable If true, enable blending.
         */
        void blend(bool enable);
        
        // [todo] isBlendingEnabled();
        
        /**
         * Set blending functions.
         * @param [in] src Source blending function.
         * @param [in] dst Destination blending function.
         */
        void blendFunc(BlendFunc src, BlendFunc dst);
        
        /**
         * Enable or disable facet culling.
         * @param [in] enable If true, cull facets based on their winding in window coordinates.
         */
        void culling(bool enable);
        
        /**
         * Check if facet culling is enabled.
         * @return true if facet culling is enabled.
         */
        bool isCullingEnabled();
        
        /**
         * Specify culling mode.
         * @param [in] mode Culling mode.
         */
        void cullingMode(CullFace mode);
        
        /**
         * Specify the orientation of front-facing polygons.
         * @param [in] clockwise If true, the polygon is considered to be
         *                       clockwise, otherwise it is counterclockwise.
         */
        void polygonWinding(bool clockwise);
        
        /**
         * Discard fragments that are outside the scissor box.
         * @param [in] enable If true, discard fragments that are outside
         *                    the scissor box.
         */
        void scissorTest(bool enable);
        /**
         * Define scissor box.
         * @param [in] pos  Lower left corner of the scissor box.
         * @param [in] size Size of the scissor box.
         */
        void scissor(glm::ivec2 const& pos, glm::ivec2 const& size);

        
        // clear
        // getters
        // cache values?
        
    private:
        Renderer();
        Renderer(Renderer const&);
        Renderer& operator=(Renderer const&); 
};

} // Render
} // Dumb

#include "renderer.inl"

#endif // _DUMB_FW_RENDERER_
