#ifndef _DUMB_FW_RENDER_CONTEXT_
#define _DUMB_FW_RENDER_CONTEXT_

#include <glm/glm.hpp>
#include <DumbFramework/config.hpp>
#include <DumbFramework/texture2d.hpp>

namespace Framework {
namespace Render    {

/** Framebuffer attachment point. **/
struct Attachment
{
    /** Framebuffer attachment point values. **/
    enum Value
    {
        DEPTH = 0,
        STENCIL,
        DEPTH_STENCIL,
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

class Context
{
    public:
        Context();
        ~Context();
        
        bool create();
        void destroy();
        bool compile();
        
        bool bind();
        void unbind();
        
        bool attach(Attachment point, Texture2D *target, int level=0, int layer=0);
        Texture2D* output(Attachment point);

        glm::ivec2 size() const;
        
    protected:
        struct Output
        {
            GLenum     attachment;
            Texture2D *target;
            GLint      level;
            GLint      layer;
            
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
        
        size_t  _outputCapacity;
        size_t  _outputCount;
        int*    _outputIndex;
        Output* _outputs;
};

} // Render
} // Framework

#include "rendercontext.inl"

#endif // _DUMB_FW_RENDER_CONTEXT_
