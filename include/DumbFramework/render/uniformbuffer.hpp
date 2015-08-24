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
#ifndef _DUMB_FW_UNIFORM_BUFFER_
#define _DUMB_FW_UNIFORM_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/render/bufferobject.hpp>

namespace Dumb   {
namespace Render {

/**
 * @brief Uniform buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Dumb::BufferObject::Detail
 * [todo] description
 */
class UniformBuffer : public BufferObject::Detail<BufferObject::UNIFORM_BUFFER>
{
    public:
        /** Constructor. **/
        UniformBuffer();
        /** Destructor. **/
        virtual ~UniformBuffer();
        /** 
         * Bind buffer to indexed buffer target.
         * @param [in] index Binding point index.
         */
        inline void bindTarget(unsigned int index);
};


/** 
 * Bind buffer to indexed buffer target.
 * @param [in] index Binding point index.
 */
void UniformBuffer::bindTarget(unsigned int index)
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(!isValid())
    {
        Log_Warning(Module::Render, "You are trying to bind an invalid %s buffer!", _infos.name);
    }
    else
    {
        GLuint bufferId;
        glGetIntegerv(_infos.query, (GLint*)&bufferId);
        if(bufferId && (bufferId != _id))
        {
            Log_Warning(Module::Render, "%s buffer %d is currently bound.", _infos.name);
        }
    }
#endif // SANITY_CHECK
    glBindBufferBase(_infos.target, index, _id);
}


} // Render
} // Dumb

#endif /* _DUMB_FW_UNIFORM_BUFFER_ */
