#ifndef _DUMB_FW_TEXTURE_
#define _DUMB_FW_TEXTURE_

#include <GL/glew.h>
#include <DumbFramework/config.hpp>

#ifndef GL_MIRROR_CLAMP_TO_EDGE
#define GL_MIRROR_CLAMP_TO_EDGE GL_MIRROR_CLAMP_TO_EDGE_EXT
#endif

namespace Framework {
namespace Texture   {
    /** Pixel format. **/
    struct PixelFormat
    {
        enum Value
        {
            UNKNOWN,
            RGB_8,
            RGBA_8,
            LUMINANCE_8,
            LUMINANCE_16
        } value;
        inline PixelFormat() : value(UNKNOWN) {}
        inline PixelFormat(Value v) : value(v) {}
        inline operator Value() { return value; }
        inline size_t bytesPerPixel() const;
        inline size_t componentCount() const;
    };
    /** Texel minification filter. **/
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
        } value;
        inline MinFilter() : value(NEAREST_TEXEL) {}
        inline MinFilter(Value v) : value(v) {}
        inline operator Value() { return value; }
        inline operator GLint();
    };
    /** Texel magnification filter. **/
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
    /** Texture wrap. **/
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

    /** Returns number of bytes per pixel. **/
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
    /** Returns number of components per pixel. **/
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
    
    /** Return the associated OpenGL value. **/
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
	/** Return the associated OpenGL value. **/
    MagFilter::operator GLint()
    {
		static const GLenum glMagFilter[] =
		{
			GL_NEAREST,
			GL_LINEAR
		};
		return glMagFilter[value];
	}
	/** Return the associated OpenGL value. **/
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
