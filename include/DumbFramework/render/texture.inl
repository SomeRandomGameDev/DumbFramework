namespace Framework {
namespace Render    {
namespace Texture   {

    size_t PixelFormat::bytesPerPixel() const
    {
        switch(value)
        {
            case UNKNOWN:
                return 0;
            case RGB_8:
                return 3;
            case RGBA_8:
                return 4;
            case RGB_16F:
                return 6;
            case RGBA_16F:
                return 8;
            case RGB_32F:
                return 12;
            case RGBA_32F:
                return 16;
            case LUMINANCE_8:
                return 1;
            case LUMINANCE_16:
                return 2;
            case DEPTH_16:
                return 2;
            case DEPTH_24:
                return 3;
            case DEPTH_32:
                return 4;
            case DEPTH_24_STENCIL_8:
                return 4;
            case DEPTH_32_STENCIL_8:
                return 5;
        };
    }

    size_t PixelFormat::componentCount() const
    {
        switch(value)
        {
            case UNKNOWN:
                return 0;
            case RGB_8:
            case RGB_16F:
            case RGB_32F:
                return 3;
            case RGBA_8:
            case RGBA_16F:
            case RGBA_32F:
                return 4;
            case LUMINANCE_8:
            case LUMINANCE_16:
            case DEPTH_16:
            case DEPTH_24:
            case DEPTH_32:
                return 1;
            case DEPTH_24_STENCIL_8:
            case DEPTH_32_STENCIL_8:
                return 2;
        };
    }
    
    /** Get the corresponding OpenGL internal format. **/
    GLint PixelFormat::internalFormat() const
    {
        switch(value)
        {
            case Texture::PixelFormat::RGB_8:
                return GL_RGB8;
            case Texture::PixelFormat::RGBA_8:
                return GL_RGBA8;
            case Texture::PixelFormat::RGB_16F:
                return GL_RGB16F;
            case Texture::PixelFormat::RGBA_16F:
                return GL_RGBA16F;
            case Texture::PixelFormat::RGB_32F:
                return GL_RGB32F;
            case Texture::PixelFormat::RGBA_32F:
                return GL_RGBA32F;
            case Texture::PixelFormat::LUMINANCE_8:
                return GL_R8;
            case Texture::PixelFormat::LUMINANCE_16:
                return GL_R16;
            case Texture::PixelFormat::DEPTH_16:
                return GL_DEPTH_COMPONENT16;
            case Texture::PixelFormat::DEPTH_24:
                return GL_DEPTH_COMPONENT24;
            case Texture::PixelFormat::DEPTH_32:
                return GL_DEPTH_COMPONENT32;
            case Texture::PixelFormat::DEPTH_24_STENCIL_8:
                return GL_DEPTH24_STENCIL8;
            case Texture::PixelFormat::DEPTH_32_STENCIL_8:
                return GL_DEPTH32F_STENCIL8;
            default:
                return GL_RGB8;
        };
    }

    /** Get the corresponding OpenGL data format. **/
    GLenum PixelFormat::format() const
    {
        switch(value)
        {
            case Texture::PixelFormat::RGB_8:
            case Texture::PixelFormat::RGB_16F:
            case Texture::PixelFormat::RGB_32F:
                return GL_RGB;
            case Texture::PixelFormat::RGBA_8:
            case Texture::PixelFormat::RGBA_16F:
            case Texture::PixelFormat::RGBA_32F:
                return GL_RGBA;
            case Texture::PixelFormat::LUMINANCE_8:
            case Texture::PixelFormat::LUMINANCE_16:
                return GL_RED;
            case Texture::PixelFormat::DEPTH_16:
                return GL_DEPTH_COMPONENT;
            case Texture::PixelFormat::DEPTH_24:
                return GL_DEPTH_COMPONENT;
            case Texture::PixelFormat::DEPTH_32:
                return GL_DEPTH_COMPONENT;
            case Texture::PixelFormat::DEPTH_24_STENCIL_8:
                return GL_DEPTH_STENCIL;
            case Texture::PixelFormat::DEPTH_32_STENCIL_8:
                return GL_DEPTH_STENCIL;
            case Texture::PixelFormat::UNKNOWN:
            default:
                return GL_RGB;
        };
    }
    /** Get the corresponding OpenGL data type. **/
    GLenum PixelFormat::type() const
    {
        switch(value)
        {
            case Texture::PixelFormat::LUMINANCE_16:
                return GL_UNSIGNED_SHORT;
            default:
                return GL_UNSIGNED_BYTE;
                break;
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

    bool operator== (PixelFormat const& p0, PixelFormat const& p1)
    {   return (p0.value == p1.value);   }
    bool operator== (PixelFormat::Value const& v, PixelFormat const& p0)
    {   return (p0.value == v);   }
    bool operator== (PixelFormat const& p0, PixelFormat::Value const& v)
    {   return (p0.value == v);   }
    bool operator!= (PixelFormat::Value const& v, PixelFormat const& p0)
    {   return (p0.value != v);   }
    bool operator!= (PixelFormat const& p0, PixelFormat::Value const& v)
    {   return (p0.value != v);   }
    bool operator!= (PixelFormat const& p0, PixelFormat const& p1)
    {   return (p0.value != p1.value);   }
    
    bool operator== (MinFilter const& f0, MinFilter const& f1)
    {   return (f0.value == f1.value);   }
    bool operator== (MinFilter::Value const& v, MinFilter const& f0)
    {   return (f0.value == v);   }
    bool operator== (MinFilter const& f0, MinFilter::Value const& v)
    {   return (f0.value == v);   }
    bool operator!= (MinFilter const& f0, MinFilter const& f1)
    {   return (f0.value != f1.value); }
    bool operator!= (MinFilter::Value const& v, MinFilter const& f0)
    {   return (f0.value != v);   }
    bool operator!= (MinFilter const& f0, MinFilter::Value const& v)
    {   return (f0.value != v);   }
    
    bool operator== (MagFilter const& f0, MagFilter const& f1)
    {   return (f0.value == f1.value);   }
    bool operator== (MagFilter::Value const& v, MagFilter const& f0)
    {   return (f0.value == v);   }
    bool operator== (MagFilter const& f0, MagFilter::Value const& v)
    {   return (f0.value == v);   }
    bool operator!= (MagFilter const& f0, MagFilter const& f1)
    {   return (f0.value != f1.value);   }
    bool operator!= (MagFilter::Value const& v, MagFilter const& f0)
    {   return (f0.value != v);   }
    bool operator!= (MagFilter const& f0, MagFilter::Value const& v)
    {   return (f0.value != v);   }
    
    bool operator== (Wrap const& w0, Wrap const& w1)
    {   return (w0.value == w1.value);   }
    bool operator== (Wrap::Value const& v, Wrap const& w0)
    {   return (w0.value == v);   }
    bool operator== (Wrap const& w0, Wrap::Value const& v)
    {   return (w0.value == v);   }
    bool operator!= (Wrap const& w0, Wrap const& w1)
    {   return (w0.value != w1.value);   }
    bool operator!= (Wrap::Value const& v, Wrap const& w0)
    {   return (w0.value != v);   }
    bool operator!= (Wrap const& w0, Wrap::Value const& v)
    {   return (w0.value != v);   }

} // Texture
} // Render
} // Framework
