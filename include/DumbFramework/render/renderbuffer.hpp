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
#ifndef _DUMB_FW_RENDERBUFFER_
#define _DUMB_FW_RENDERBUFFER_

#include <glm/glm.hpp>
#include <DumbFramework/render.hpp>
#include <DumbFramework/render/texture.hpp>

namespace Dumb   {
namespace Render {

/**
 * Renderbuffer.
 * @ingroup DUMB_FW_TEXTURE
 */
class Renderbuffer
{
    public:
        /** Constructor. **/
        Renderbuffer();
        /**Destructor. **/
        ~Renderbuffer();
        /** 
         * Create renderbuffer.
         * @param [in] size    Renderbuffer size.
         * @param [in] format  Renderbuffer format.
         * @param [in] samples Samples count (default=-0).
         * @return true if the renderbufer was succesfully created.
         */
        bool create(const glm::ivec2& size, Texture::PixelFormat format, size_t samples=0);
        /**
         * Destroy renderbuffer.
         */
        void destroy();
        /**
         * Tell if the renderbuffer is valid.
         * @return true if the renderbuffer is valid.
         */
        bool isValid() const;
        /**
         * Bind renderbuffer.
         */
        void bind() const;
        /**
         * Unbind renderbuffer.
         */
        void unbind() const;
        /**
         * Check if the renderbuffer is bound.
         * @return true if the renderbuffer is bound.
         */
        bool isBound() const;
        /**
         * Unbind currently bound renderbuffers.
         */
        static void unbindAll();
        /**
         * Get renderbuffer width and height
         * @return renderbuffer width and height as a glm::ivec2
         */
        const glm::ivec2& size() const;
        /**
         * Get renderbuffer sample count.
         * @return sample count.
         */
         size_t samples() const;
        /**
         * Get renderbuffer pixel format.
         * @return pixel format.
         */
        const Texture::PixelFormat& pixelFormat() const;
        /**
         * Get renderbuffer id.
         * @return renderbuffer id.
         */
        GLuint id() const;
    private:
        glm::ivec2 _size;             /**< Renderbuffer width and height. **/
        size_t     _samples;          /**< Renderbuffer sample count.     **/
        Texture::PixelFormat _format; /**< Pixel format.                  **/
        GLuint _id;                   /**< Resource id.                   **/
};

} // Render
} // Dumb

#endif /* _DUMB_FW_RENDERBUFFER_ */
