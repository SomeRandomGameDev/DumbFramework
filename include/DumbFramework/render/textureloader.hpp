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
#ifndef _DUMB_FW_TEXTURE_LOADER_
#define _DUMB_FW_TEXTURE_LOADER_

#include <initializer_list>
#include <string>
#include <vector>
#include <external/stb_image.h>
#include <DumbFramework/render/texture2d.hpp>

namespace Dumb    {
namespace Render  {
namespace Texture {
/**
 * Create texture from file.
 * @ingroup DUMB_FW_TEXTURE
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
 * @ingroup DUMB_FW_TEXTURE
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
 * @ingroup DUMB_FW_TEXTURE
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
 * @ingroup DUMB_FW_TEXTURE
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
} // Render
} // Dumb

#endif /* _DUMB_FW_TEXTURE_LOADER_ */
