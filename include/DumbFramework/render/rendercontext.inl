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
namespace Dumb   {
namespace Render {

/** Constructs attachment point from value. **/
void Attachment::from(GLenum v)
{
    switch(v)
    {
        case GL_DEPTH_ATTACHMENT:
            value = Attachment::DEPTH;
        break;
        
        case GL_STENCIL_ATTACHMENT:
            value = Attachment::STENCIL;
        break;
        
        case GL_DEPTH_STENCIL_ATTACHMENT:
            value = Attachment::DEPTH_STENCIL;
        break;
        
        default:
            value = Attachment::COLOR;
            if(v >= GL_COLOR_ATTACHMENT0)
            {
                value += (v - GL_COLOR_ATTACHMENT0);
            }
        break;
    }
}
/** Convert to OpenGL compliant value. **/
GLenum Attachment::to() const
{
    switch(value)
    {
        case Attachment::DEPTH:
            return GL_DEPTH_ATTACHMENT;
        case Attachment::STENCIL:
            return GL_STENCIL_ATTACHMENT;
        case Attachment::DEPTH_STENCIL:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        default:
            return GL_COLOR_ATTACHMENT0 + (value - Attachment::COLOR);
    }
}
bool operator== (Attachment const& a0, Attachment const& a1)
{ return (a0.value == a1.value); }
bool operator== (Attachment::Value const& v, Attachment const& a0)
{ return (static_cast<unsigned int>(v) == a0.value); }
bool operator== (unsigned int v, Attachment const& a0)
{ return (v == a0.value); }
bool operator== (Attachment const& a0, Attachment::Value const& v)
{ return (a0.value == static_cast<unsigned int>(v)); }
bool operator== (Attachment const& a0, unsigned int v)
{ return (a0.value == v); }
bool operator!= (Attachment const& a0, Attachment const& a1)
{ return (a0.value != a1.value); }
bool operator!= (Attachment::Value const& v, Attachment const& a0)
{ return (static_cast<unsigned int>(v) != a0.value); }
bool operator!= (unsigned int v, Attachment const& a0)
{ return (v != a0.value); }
bool operator!= (Attachment const& a0, Attachment::Value const& v)
{ return (a0.value != static_cast<unsigned int>(v)); }
bool operator!= (Attachment const& a0, unsigned int v)
{ return (a0.value != v); }


bool Context::TargetParameter::complete() const
{
    return ((GL_NONE != attachment) && ((nullptr != texture) || (nullptr != renderbuffer)));
}

void Context::TargetParameter::attach() const
{
    if(nullptr != texture)
    {
        if(texture->layerCount() > 1)
        {
            glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, texture->id(), level, layer);
        }
        else
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->id(), level);
        }
    }
    else if(nullptr != renderbuffer)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer->id());
    }
}

} // Render
} // Dumb
