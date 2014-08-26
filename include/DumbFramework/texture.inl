namespace Framework {
namespace Texture   {

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
