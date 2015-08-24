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
#ifndef _DUMB_FW_TEXTURE_
#define _DUMB_FW_TEXTURE_

#include <GL/glew.h>
#include <DumbFramework/config.hpp>

#ifndef GL_MIRROR_CLAMP_TO_EDGE
#define GL_MIRROR_CLAMP_TO_EDGE GL_MIRROR_CLAMP_TO_EDGE_EXT
#endif

namespace Dumb   {
namespace Render {

/**
 * @defgroup DUMB_FW_TEXTURE Texture management.
 * @ingroup DUMB_FW_RENDERING
 * GPU Texture management.
 */
namespace Texture   {
/** 
 * Pixel format. 
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
        /** Red, Green, Blue with 16 bits per component (float). **/
        RGB_16F,
        /** Red, Green, Blue, Alpha with 16 bits per component (float). **/
        RGBA_16F,
        /** Red, Green, Blue with 32 bits per component (float). **/
        RGB_32F,
        /** Red, Green, Blue, Alpha with 32 bits per component (float). **/
        RGBA_32F,
        /** Luminance 8 bits (1 component). **/
        LUMINANCE_8,
        /** Luminance 16 bits (1 component). **/
        LUMINANCE_16,
        /** Depth 16 bits (1 component). **/
        DEPTH_16,
        /** Depth 24 bits (1 component). **/
        DEPTH_24,
        /** Depth 32 bits (1 component). **/
        DEPTH_32,
        /** Depth 24 bits and 8 bits stencil (2 component). **/
        DEPTH_24_STENCIL_8,
        /** Depth 32 bits and 8 bits stencil (2 component). **/
        DEPTH_32_STENCIL_8
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
     *  Pixel format       | Bpp
     *  ------------------ | :--:
     *  RGB_8              | 3
     *  RGBA_8             | 4
     *  RGB_16F            | 6
     *  RGBA_16F           | 8
     *  RGB_32F            | 12
     *  RGBA_32F           | 16
     *  LUMINANCE_8        | 1
     *  LUMINANCE_16       | 2
     *  DEPTH_16           | 2
     *  DEPTH_24           | 3
     *  DEPTH_32           | 4
     *  DEPTH_24_STENCIL_8 | 4
     *  DEPTH_32_STENCIL_8 | 5
     * 
     * @return Bytes per pixel. 
     */
    inline size_t bytesPerPixel() const;
    /**
     * @brief Get component count.
     * 
     * Returns the number of color components associated with current
     * pixel format.
     *  Pixel format       | Components
     *  ------------------ | :--------:
     *  RGB_8              | 3
     *  RGBA_8             | 4
     *  RGB_16F            | 3
     *  RGBA_16F           | 4
     *  RGB_32F            | 3
     *  RGBA_32F           | 4
     *  LUMINANCE_8        | 1
     *  LUMINANCE_16       | 1
     *  DEPTH_16           | 1
     *  DEPTH_24           | 1
     *  DEPTH_32           | 1
     *  DEPTH_24_STENCIL_8 | 2
     *  DEPTH_32_STENCIL_8 | 2
     * 
     * @return Component count.
     */
    inline size_t componentCount() const;
    /** Get the corresponding OpenGL internal format. **/
    inline GLint internalFormat() const;
    /** Get the corresponding OpenGL data format. **/
    inline GLenum format() const;
    /** Get the corresponding OpenGL data type. **/
    inline GLenum type() const;
};
inline bool operator== (PixelFormat const& p0, PixelFormat const& p1);
inline bool operator== (PixelFormat::Value const& v, PixelFormat const& p0);
inline bool operator== (PixelFormat const& p0, PixelFormat::Value const& v);
inline bool operator!= (PixelFormat const& p0, PixelFormat const& p1);
inline bool operator!= (PixelFormat::Value const& v, PixelFormat const& p0);
inline bool operator!= (PixelFormat const& p0, PixelFormat::Value const& v);

/** 
 * Texel minification filter. 
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
inline bool operator== (MinFilter const& f0, MinFilter const& f1);
inline bool operator== (MinFilter::Value const& v, MinFilter const& f0);
inline bool operator== (MinFilter const& f0, MinFilter::Value const& v);
inline bool operator!= (MinFilter const& f0, MinFilter const& f1);
inline bool operator!= (MinFilter::Value const& v, MinFilter const& f0);
inline bool operator!= (MinFilter const& f0, MinFilter::Value const& v);
/** 
 * Texel magnification filter. 
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
inline bool operator== (MagFilter const& f0, MagFilter const& f1);
inline bool operator== (MagFilter::Value const& v, MagFilter const& f0);
inline bool operator== (MagFilter const& f0, MagFilter::Value const& v);
inline bool operator!= (MagFilter const& f0, MagFilter const& f1);
inline bool operator!= (MagFilter::Value const& v, MagFilter const& f0);
inline bool operator!= (MagFilter const& f0, MagFilter::Value const& v);
/** 
 * Texture wrap. 
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
inline bool operator== (Wrap const& w0, Wrap const& w1);
inline bool operator== (Wrap::Value const& v, Wrap const& w0);
inline bool operator== (Wrap const& w0, Wrap::Value const& v);
inline bool operator!= (Wrap const& w0, Wrap const& w1);
inline bool operator!= (Wrap::Value const& v, Wrap const& w0);
inline bool operator!= (Wrap const& w0, Wrap::Value const& v);

} // Texture
} // Render
} // Dumb

#include "texture.inl"

#endif /* _DUMB_FW_TEXTURE_ */
