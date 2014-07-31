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
     * @todo
     */
namespace Texture   {
    /** 
     * @brief Pixel format. 
     * @ingroup DUMB_FW_TEXTURE
     */
    struct PixelFormat
    {
        enum Value
        {
            UNKNOWN,        /**< Unknown pixel format. **/
            RGB_8,          /**< Red, Green, Blue with 8 bits per component. **/
            RGBA_8,         /**< Red, Green, Blue, Alpha with 8 bits per component. **/
            LUMINANCE_8,    /**< Luminance 8 bits (1 component). **/
            LUMINANCE_16    /**< Luminance 16 bits (1 component). **/
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
        enum Value
        {
            NEAREST_TEXEL,
            LINEAR_TEXEL,
            NEAREST_TEXEL_NEAREST_MIPMAP,
            NEAREST_TEXEL_LINEAR_MIPMAP,
            LINEAR_TEXEL_NEAREST_MIPMAP,
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
        enum Value
        {
            NEAREST_TEXEL,
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
        enum Value
        {
            REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER,
            MIRRORED_REPEAT,
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
