#include <DumbFramework/log.hpp>

#include <DumbFramework/render/textureloader.hpp>

namespace Framework {
namespace Render    {
namespace Texture   {

// [todo] Should be moved to pixel format constructor.
PixelFormat compToFormat(int comp)
{
    PixelFormat format;
    switch(comp)
    {
        case 1:
            return PixelFormat::LUMINANCE_8;
        case 3:
            return PixelFormat::RGB_8;
        case 4:
            return PixelFormat::RGBA_8;
        default:
            return PixelFormat::UNKNOWN;
    }
}
/**
 * Create texture from file.
 * @param [out] out     Output texture.
 * @param [in]  filname Image filename.
 * @param [in]  format  (Optional) Expected pixel format. If set to
 *                      UNKNOWN, the image pixel format will be used.
 *                      Otherwise the image pixel format must match the
 *                      one provided.
 * @param [in]  size    (Optional) Expected image size.
 */
bool load(Texture2D& out, std::string const& filename, PixelFormat const& format, glm::ivec2 const& size)
{
    unsigned char *data;
    glm::ivec2 imageSize;
    int comp;
    
    data = stbi_load(filename.c_str(), &imageSize.x, &imageSize.y, &comp, 0);
    if(nullptr == data)
    {
        Log_Error(Module::Base, "Failed to load image: %s", filename.c_str());
        return false;
    }

    bool ret = true;

    PixelFormat imageFormat;
    switch(comp)
    {
        case 1:
            imageFormat = PixelFormat::LUMINANCE_8;
            break;
        case 3:
            imageFormat = PixelFormat::RGB_8;
            break;
        case 4:
            imageFormat = PixelFormat::RGBA_8;
            break;
        default:
            ret = false;
            Log_Error(Module::Base, "Unsupported pixel format (%s)", filename.c_str());
            break;
    }
    
    if((PixelFormat::UNKNOWN != format) &&
       (imageFormat != format))
    {
        Log_Error(Module::Base, "Invalid pixel format (%s)", filename.c_str());
        ret = false;
    }
    if((-1 != size.x) && (imageSize.x != size.x))
    {
        Log_Error(Module::Base, "Invalid width. Expected %d but is %d (%s)", size.x, imageSize.x, filename.c_str());
        ret = false;
    }
    if((-1 != size.y) && (imageSize.y != size.y))
    {
        Log_Error(Module::Base, "Invalid height. Expected %d but is %d (%s)", size.y, imageSize.y, filename.c_str());
        ret = false;
    }
    if(ret)
    {
        ret = out.create(imageSize, imageFormat);
        if(false == ret)
        {
            Log_Error(Module::Base, "Failed to create texture.");
        }
        else
        {
            ret = out.setData(data);
            if(false == ret)
            {
                Log_Error(Module::Base, "Failed to set texture data.");
            }
        }
    }
    
    stbi_image_free(data);
    return ret;
}
/**
 * Load a single texture layer from file.
 * @param [out] out      Output texture.
 * @param [in]  filename Image filename.
 * @param [in]  layer    Layer in which the image will be loaded (default = 0).
 * @return @b true if the layer was successfully loaded, or @b false if
 *            the image failed to load or if the image description (size, pixelformat)
 *            foes not match the texture.
 */
bool loadLayer(Texture2D& out, std::string const& filename, int layer)
{
    if(layer >= out.layerCount())
    {
        Log_Error(Module::Base, "Invalid layer %d (max: %d)", layer, out.layerCount());
        return false;
    }
    
    unsigned char *data;
    glm::ivec2 size;
    int comp;
    bool ret = true;
    
    data = stbi_load(filename.c_str(), &size.x, &size.y, &comp, 0);
    if(nullptr == data)
    {
        Log_Error(Module::Base, "Failed to load image: %s", filename.c_str());
        return false;
    }
    
    if(size != out.size())
    {
        Log_Error(Module::Base, "Incorrect image size %s: [%d,%d], texture: [%d;%d]", 
                                filename.c_str(), size.x, size.y, out.size().x, out.size().y);
        ret = false;
    }

    PixelFormat format = compToFormat(comp);
    if(format != out.pixelFormat())
    {
        Log_Error(Module::Base, "Incorrect pixel format %s: %x, texture: %x", 
                                filename.c_str(), format, out.pixelFormat());
        ret = false;
    }

    if(ret)
    {
        ret = out.setData(data, layer);
        if(false == ret)
        {
            Log_Error(Module::Base, "Failed to set texture data.");
        }
    }

    stbi_image_free(data);
    return ret;
}
/**
 * Create texture array from files.
 * @param [out] out          Output texture.
 * @param [in]  filenameList Image filenames.
 * @param [in]  format  (Optional) Expected pixel format. If set to
 *                      UNKNOWN, the image pixel format will be used.
 *                      Otherwise the image pixel format must match the
 *                      one provided.
 * @param [in]  size    (Optional) Expected image size.
 * @return @b true if the images were successfully loaded, or @b false if
 *            one of the image failed to load or if the image description
 *            does not match.
 */
bool load(Texture2D& out, std::initializer_list<std::string> filenameList, PixelFormat const& format, glm::ivec2 const& size)
{
    return load(out, std::vector<std::string>(filenameList), format, size);
}
/**
 * Create texture array from files.
 * @param [out] out          Output texture.
 * @param [in]  filenameList Image filenames.
 * @param [in]  format  (Optional) Expected pixel format. If set to
 *                      UNKNOWN, the image pixel format will be used.
 *                      Otherwise the image pixel format must match the
 *                      one provided.
 * @param [in]  size    (Optional) Expected image size.
 * @return @b true if the images were successfully loaded, or @b false if
 *            one of the image failed to load or if the image description
 *            does not match.
 */
bool load(Texture2D& out, std::vector<std::string> const& filenameList, PixelFormat const& format, glm::ivec2 const& size)
{
    int comp;
    unsigned char *data;
    glm::ivec2 imageSize;

    bool ret;

    data = stbi_load(filenameList[0].c_str(), &imageSize.x, &imageSize.y, &comp, 0);
    if(nullptr == data)
    {
        Log_Error(Module::Base, "Failed to load image: %s", filenameList[0].c_str());
        return false;
    }
    
    PixelFormat imageFormat = compToFormat(comp);
    if((PixelFormat::UNKNOWN != format) &&
       (imageFormat != format))
    {
        Log_Error(Module::Base, "Invalid pixel format (%s)", filenameList[0].c_str());
        ret = false;
    }
    if((-1 != size.x) && (imageSize.x != size.x))
    {
        Log_Error(Module::Base, "Invalid width. Expected %d but is %d (%s)", size.x, imageSize.x, filenameList[0].c_str());
        ret = false;
    }
    if((-1 != size.y) && (imageSize.y != size.y))
    {
        Log_Error(Module::Base, "Invalid height. Expected %d but is %d (%s)", size.y, imageSize.y, filenameList[0].c_str());
        ret = false;
    }
    Log_Info(Module::Base, "Texture Size : %dx%d", imageSize.x, imageSize.y);
    ret = out.create(imageSize, imageFormat, filenameList.size());
    if(ret)
    {
        ret = out.setData(data, 0);
        if(false == ret)
        {
            Log_Error(Module::Base, "Failed to set texture data for layer 0.");
        }
    }
    else
    {
        Log_Error(Module::Base, "Failed to create texture.");
    }

    for(size_t i=1; ret && (i<filenameList.size()); i++)
    {
        ret = loadLayer(out, filenameList[i], i);
        if(false == ret)
        {
            Log_Error(Module::Base, "Failed to load layer %d.", i);
        }
    }
    return ret;
}


} // Texture
} // Render
} // Framework
