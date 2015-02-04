#ifndef _DUMB_FW_UNIFORM_BUFFER_
#define _DUMB_FW_UNIFORM_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/render/bufferobject.hpp>

namespace Framework {
namespace Render    {

/**
 * @brief Uniform buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Framework::BufferObject::Detail
 * [todo] description
 */
class UniformBuffer : public BufferObject::Detail<BufferObject::UNIFORM_BUFFER>
{
    public:
        /** Constructor. **/
        UniformBuffer() : BufferObject::Detail<BufferObject::UNIFORM_BUFFER>() {}
        /** Destructor. **/
        virtual ~UniformBuffer() {}
        /** 
         * Bind buffer to indexed buffer target.
         * @param [in] index Binding point index.
         */
        inline void bindTarget(unsigned int index)
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

};

} // Render
} // Framework

#endif /* _DUMB_FW_UNIFORM_BUFFER_ */
