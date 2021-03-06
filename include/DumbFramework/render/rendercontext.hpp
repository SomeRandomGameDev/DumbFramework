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
#ifndef _DUMB_FW_RENDER_CONTEXT_
#define _DUMB_FW_RENDER_CONTEXT_

#include <glm/glm.hpp>
#include <DumbFramework/config.hpp>
#include <DumbFramework/render/texture2d.hpp>
#include <DumbFramework/render/renderbuffer.hpp>

namespace Dumb   {
namespace Render {

/** Framebuffer attachment point. **/
struct Attachment
{
    /** Framebuffer attachment point values. **/
    enum Value
    {
        /** Depth attachment. **/
        DEPTH = 0,
        /** Stencil attachment. **/
        STENCIL,
        /** 
         * Both depth and stencil values will be written to attached
         * element.
         */
        DEPTH_STENCIL,
        /** Color attachments. **/
        COLOR
    };
    unsigned int value; /**< Framebuffer attachment point value. **/
    /**
     * @brief Default constructor.
     */
    inline Attachment() : value(COLOR) {}
    /**
     * @brief Constructor.
     * Constructs attachment point from value.
     */
    inline Attachment(Value v) : value(v) {}
    /**
     * @brief Constructor.
     * Constructs attachment point from value.
     */
    inline Attachment(unsigned int v) : value(v) {}
    inline operator unsigned() { return value; }
    inline operator const unsigned int() const { return value; }
    /** Constructs attachment point from OpenGL value. **/
    inline void from(GLenum v);
    /** Convert to OpenGL compliant value. **/
    inline GLenum to() const;
};
inline bool operator== (Attachment const& a0, Attachment const& a1);
inline bool operator== (Attachment::Value const& v, Attachment const& a0);
inline bool operator== (unsigned int v, Attachment const& a0);
inline bool operator== (Attachment const& a0, Attachment::Value const& v);
inline bool operator== (Attachment const& a0, unsigned int v);
inline bool operator!= (Attachment const& a0, Attachment const& a1);
inline bool operator!= (Attachment::Value const& v, Attachment const& a0);
inline bool operator!= (unsigned int v, Attachment const& a0);
inline bool operator!= (Attachment const& a0, Attachment::Value const& v);
inline bool operator!= (Attachment const& a0, unsigned int v);

/**
 * Render context.
 * [todo]
 */
class Context
{
    public:
        /** Default constructor. **/
        Context();
        /** Destructor. **/
        ~Context();
        
        bool create();
        void destroy();
        bool compile();
        
        bool bind();
        void unbind();
        
        bool attach(Attachment point, Texture2D *texture, int level=0, int layer=0);
        bool attach(Attachment point, Renderbuffer *renderbuffer);

        Texture2D*    getTexture(Attachment point);
        Renderbuffer* getRenderbuffer(Attachment point);

        glm::ivec2 size() const;
        
    protected:
        bool addNewAttachment(Attachment point,  glm::ivec2 const& size);

    protected:
        struct TargetParameter
        {
            GLenum     attachment;
            GLint      level;
            GLint      layer;
            
            Texture2D    *texture;
            Renderbuffer *renderbuffer;
            
            inline bool complete() const;
            inline void attach() const;
        };
        
        enum InternalState
        {
            UNITIALIZED = 0,
            COMPILATION_NEEDED,
            READY
        };
        
    protected:
        InternalState _state;
        GLuint _framebuffer;
        
        size_t  _colorAttachmentCount;
        GLenum* _colorAttachments;
        
        size_t  _targetCapacity;
        size_t  _targetCount;
        int*    _targetIndex;
        TargetParameter* _targets;
};

} // Render
} // Dumb

#include "rendercontext.inl"

#endif // _DUMB_FW_RENDER_CONTEXT_
