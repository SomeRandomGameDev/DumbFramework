#ifndef _DUMB_FW_TEXTURE_
#define _DUMB_FW_TEXTURE_

#include <GL/glew.h>
#include <DumbFramework/config.hpp>

#ifndef GL_MIRROR_CLAMP_TO_EDGE
#define GL_MIRROR_CLAMP_TO_EDGE GL_MIRROR_CLAMP_TO_EDGE_EXT
#endif

namespace Framework {
/**
 * @defgroup DUMB_FW_TEXTURE Texture management.
 * @ingroup DUMB_FW_RENDERING
 * GPU Texture management.
 */
namespace Texture   {
/** 
 * @brief Pixel format. 
 * @ingroup DUMB_FW_TEXTURE
 */
struct PixelFormat
{
    /** Pixel formats. **/
    enum Value
    {
        /** Unknown pixel format. **/
        UNKNOWN,
        /** Red, Green, Blue with 8 bits per component. **/
        RGB_8,
        /** Red, Green, Blue, Alpha with 8 bits per component. **/
        RGBA_8,
        /** Luminance 8 bits (1 component). **/
        LUMINANCE_8,
        /** Luminance 16 bits (1 component). **/
        LUMINANCE_16
    };
    Value value; /**< Pixel format value. **/
    /** 
     * @brief Default constructor.
     * By default the pixel format is @c UNKNOWN.
     */
    inline PixelFormat() : value(UNKNOWN) {}
    /**
     * @brief Constructor.
     * @param [in] v  Pixel format value.
     */
    inline PixelFormat(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** 
     * @brief Get Bytes per pixel.
     * 
     * Returns the number of bytes per pixel associated with current
     * pixel format. 
     *  Pixel format | Bpp
     *  ------------ | :--:
     *  RGB_8        | 3
     *  RGBA_8       | 4
     *  LUMINANCE_8  | 1
     *  LUMINANCE_16 | 2
     * 
     * @return Bytes per pixel. 
     */
    inline size_t bytesPerPixel() const;
    /**
     * @brief Get component count.
     * 
     * Returns the number of color components associated with current
     * pixel format.
     *  Pixel format | Components
     *  ------------ | :--------:
     *  RGB_8        | 3
     *  RGBA_8       | 4
     *  LUMINANCE_8  | 1
     *  LUMINANCE_16 | 1
     * @return Component count.
     */
    inline size_t componentCount() const;
};
/** 
 * @brief Texel minification filter. 
 * @ingroup DUMB_FW_TEXTURE
 */
struct MinFilter
{
    /** Minification filter values. **/
    enum Value
    {
        /**
         * Return the texture element that is the nearest to the 
         * specified texture coordinate.
         */
        NEAREST_TEXEL,
        /**
         * Return the weighted average of the four texture elements
         * that are closest to the specified texture coordinate.
         */
        LINEAR_TEXEL,
        /**
         * Chooses the mipmap that most closely matches the size of 
         * the pixel being textured, and returns the texture element
         * in that mipmap that is the nearest to the specified
         * texture coordinate.
         */
        NEAREST_TEXEL_NEAREST_MIPMAP,
        /**
         * Chooses the two mipmaps that most closely match the size 
         * of the pixel being textured and extract the texture
         * element in those two mipmaps that is the nearest to the
         * specified texture coordinate. The final result is the
         * weighted average of those two values.
         */
        NEAREST_TEXEL_LINEAR_MIPMAP,
        /**
         * Chooses the mipmap that most closely matches the size of 
         * the pixel being textured, and returns the weighted 
         * average of the four texture element that are closest to
         * the specified texture coordinate.
         */
        LINEAR_TEXEL_NEAREST_MIPMAP,
        /**
         * Chooses the two mipmaps that most closely match the size 
         * of the pixel being textured and compute in each mipmap
         * the weighted average of the four texture elements that
         * are closest to the specified texture coordinate. The 
         * final result is the the weighted average of those two
         * values.
         */
        LINEAR_TEXEL_LINEAR_MIPMAP
    };
    Value value; /**< Minification filter value. **/
    /**
     * @brief Default constructor.
     * By default the minification filter is set to the nearest
     * texel.
     */
    inline MinFilter() : value(NEAREST_TEXEL) {}
    /**
     * @brief Constructor.
     * Constructs minification filter from value.
     */
    inline MinFilter(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** Convert to OpenGL compliant value. **/
    inline operator GLint();
};
/** 
 * @brief Texel magnification filter. 
 * @ingroup DUMB_FW_TEXTURE
 */
struct MagFilter
{
    /** Magnification filter values. **/
    enum Value
    {
        /**
         * Return the texture element that is the nearest to the 
         * specified texture coordinate.
         */
        NEAREST_TEXEL,
        /**
         * Return the weighted average of the four texture elements
         * that are closest to the specified texture coordinate.
         */
        LINEAR_TEXEL
    };
    Value value; /**< Magnification filter value. **/
    /**
     * @brief Default constructor.
     * By default the magnification filter is set to the nearest 
     * texel.
     */
    inline MagFilter() : value(NEAREST_TEXEL) {}
    /**
     * @brief Constructor.
     * Constructs magnification filter from value.
     */
    inline MagFilter(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator const Value() const { return value; }
    /** Convert to OpenGL compliant value. **/
    inline operator GLint();
};
/** 
 * @brief Texture wrap. 
 * @ingroup DUMB_FW_TEXTURE
 */
struct Wrap
{
    /** Texture wrap values. **/
    enum Value
    {
        /**
         * The integer part of the texture coodinate is ignored.
         * Therefore the texture coordinate loops between 0 and 1
         * creating a repeating pattern.
         */
        REPEAT,
        /**
         * If the texture coordinate is clamped between 
         * \f$ \left[ \frac{1}{2N}, 1 - \frac{1}{2N} \right] \f$ .
         * @c N being the size of the texture in the direction of 
         * interest.
         */
        CLAMP_TO_EDGE,
        /**
         * If the texture coordinate is outside the texture size,
         * the texel fetched is replaced by the border color.
         */
        CLAMP_TO_BORDER,
        /**
         * If the integer part of the texture coordinate is even,
         * the fractional part is used. Otherwise, if the integer
         * part is odd, the texture coordinate is set to 
         * \f$ 1 - fractional(s) \f$ . Where \f$ fractional \f$ is
         * the function returning the fractional part of a floating
         * point number.
         */
        MIRRORED_REPEAT,
        /**
         * The texture coordinate is repeated just like 
         * MIRRORED_REPEAT for one repetition and then clamped as in
         * CLAMP_TO_EDGE.
         */
        MIRROR_CLAMP_TO_EDGE
    };
    Value value; /**< Texture wrap value. **/
    /** 
     * @brief Default constructor.
     * By default the texture wrap mode is set to @c REPEAT.
     */ 
    inline Wrap() : value(REPEAT) {}
    /**
     * @brief Constructor.
     * Constructs texture wrap mode from value.
     */
    inline Wrap(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator const Value() const { return value; }
    /** Convert to OpenGL compliant value. **/
    inline operator GLint();
};

} // Texture
} // Framework

#include "texture.inl"

#endif /* _DUMB_FW_TEXTURE_ */
