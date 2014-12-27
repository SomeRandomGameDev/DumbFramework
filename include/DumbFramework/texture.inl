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
} // Framework
