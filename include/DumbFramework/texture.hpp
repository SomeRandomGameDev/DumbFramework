#ifndef _DUMB_FW_TEXTURE_
#define _DUMB_FW_TEXTURE_

#include <GL/glew.h>
#include <DumbFramework/config.hpp>

#ifndef GL_MIRROR_CLAMP_TO_EDGE
#define GL_MIRROR_CLAMP_TO_EDGE GL_MIRROR_CLAMP_TO_EDGE_EXT
#endif

namespace Framework {
    /**
     * @defgroup DUMB_FW_TEXTURE GPU texture module.
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
        /** Convert minification filter value to OpenGL compliant value. **/
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
        } value;
        inline MagFilter() : value(NEAREST_TEXEL) {}
        inline MagFilter(Value v) : value(v) {}
        inline operator Value() { return value; }
        inline operator const Value() const { return value; }
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
        } value;
        inline Wrap() : value(REPEAT) {}
        inline Wrap(Value v) : value(v) {}
        inline operator Value() { return value; }
        inline operator const Value() const { return value; }
        inline operator GLint();
    };

    inline size_t PixelFormat::bytesPerPixel() const
    {
        switch(value)
        {
            case UNKNOWN:
                return 0;
            case RGB_8:
                return 3;
            case RGBA_8:
                return 4;
            case LUMINANCE_8:
                return 1;
            case LUMINANCE_16:
                return 2;
        };
    }

    inline size_t PixelFormat::componentCount() const
    {
        switch(value)
        {
            case UNKNOWN:
                return 0;
            case RGB_8:
                return 3;
            case RGBA_8:
                return 4;
            case LUMINANCE_8:
            case LUMINANCE_16:
                return 1;
        };
    }
    
    MinFilter::operator GLint()
    {
        static const GLenum glMinFilter[] =
        { 
            GL_NEAREST,
            GL_LINEAR,
            GL_NEAREST_MIPMAP_NEAREST,
            GL_NEAREST_MIPMAP_LINEAR,
            GL_LINEAR_MIPMAP_NEAREST,
            GL_LINEAR_MIPMAP_LINEAR
        };
        return glMinFilter[value];
    }
    
    MagFilter::operator GLint()
    {
        static const GLenum glMagFilter[] =
        {
            GL_NEAREST,
            GL_LINEAR
        };
        return glMagFilter[value];
    }
    
    Wrap::operator GLint()
    {   
        static const GLenum glWrapMode[] =
        {
            GL_REPEAT,
            GL_CLAMP_TO_EDGE,
            GL_CLAMP_TO_BORDER,
            GL_MIRRORED_REPEAT,
            GL_MIRROR_CLAMP_TO_EDGE
        };
        return glWrapMode[value];
    }
} // Texture
} // Framework

#endif /* _DUMB_FW_TEXTURE_ */
