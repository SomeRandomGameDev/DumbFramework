#ifndef _DUMB_FW_TEXTURE_LOADER_
#define _DUMB_FW_TEXTURE_LOADER_

#include <initializer_list>
#include <string>
#include <vector>
#include <external/stb_image.h>
#include <DumbFramework/texture2d.hpp>

namespace Framework {
namespace Texture   {

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
bool load(Texture2D& out, std::string const& filename, PixelFormat const& format = PixelFormat::UNKNOWN, glm::ivec2 const& size = glm::ivec2(-1));
/**
 * Load a single texture layer from file.
 * @param [out] out      Output texture.
 * @param [in]  filename Image filename.
 * @param [in]  layer    Layer in which the image will be loaded (default = 0).
 * @return @b true if the layer was successfully loaded, or @b false if
 *            the image failed to load or if the image description (size, pixelformat)
 *            foes not match the texture.
 */
bool loadLayer(Texture2D& out, std::string const& filename, int layer=0);
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
bool load(Texture2D& out, std::initializer_list<std::string> filenameList, PixelFormat const& format = PixelFormat::UNKNOWN, glm::ivec2 const& size = glm::ivec2(-1));
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
bool load(Texture2D& out, std::vector<std::string> const& filenameList, PixelFormat const& format = PixelFormat::UNKNOWN, glm::ivec2 const& size = glm::ivec2(-1));

} // Texture
} // Framework

#endif /* _DUMB_FW_TEXTURE_LOADER_ */
