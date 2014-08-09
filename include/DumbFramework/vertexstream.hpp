#ifndef _DUMB_FW_VERTEX_STREAM_
#define _DUMB_FW_VERTEX_STREAM_

#include <vector>

#include <DumbFramework/vertexbuffer.hpp>
#include <DumbFramework/indexbuffer.hpp>
#include <DumbFramework/geometry.hpp>

namespace Framework {

/**
 * Vertex stream.
 * @ingroup DUMB_FW_GEOMETRY_RENDERING
 */
class VertexStream
{
    public:
        /**
         * Constructor.
         */
        VertexStream();
        /**
         * Destructor.
         */
        ~VertexStream();
        /**
         * Create vertex stream.
         * @param [in] vertexBuffer Vertex buffer that will be associated
         *                          with the current vertex stream.
         * @return true if the vertex stream was succesfully created.
         */
        bool create(VertexBuffer* vertexBuffer);
        /**
         * Destroy vertex stream.
         */
        void destroy();
        /**
         * Add attribute to vertex stream.
         * @param [in] index  Attribute index (starts at 0).
         * @param [in] type   Type of the attribute components.
         * @param [in] size   Number of components.
         * @param [in] stride Number of bytes between 2 consecutives attributes.
         * @param [in] offset Offset of the first attribute of the first vertex.
         * @return true if the attribute was successfully set.
         */
        bool set(unsigned int index, Geometry::ComponentType type, size_t size, size_t stride, size_t offset);
        /**
         * Add attribute to vertex stream.
         * @param [in] index  Attribute index (starts at 0).
         * @param [in] attr   Attribute.
         * @return true if the attribute was successfully set.
         */
        bool set(unsigned int index, Geometry::Attribute const& attr);
        /**
         * @todo
         * @return
         */
        bool compile();
        /**
         * Bind vertex stream.
         */
        void bind();
        /**
         * Unbind vertex stream.
         */
        void unbind();
        /**
         * Unbind any vertex stream.
         */
        static void unbindAll();
        /**
         * Check if the vertex stream is bound.
         * @return true if the vertex stream is bound.
         */
        bool isBound() const;
        /** 
         * @todo 
         * @param [in] primitive
         * @param [in] offset
         * @param [in] count
         */
        void draw(Geometry::Primitive primitive, size_t offset, size_t count) const;
        /**
         * Check if the attribute at the specified index was set.
         * @param [in] index Attribute index.
         * @return true if the attribute is set.
         */
        bool isAttributeSet(unsigned int index) const;
        /**
         * @todo
         */
        VertexBuffer* getVertexBuffer();
        /**
         * @todo
         */
        VertexBuffer const* getVertexBuffer() const;

    private:
        /** Vertex array identifier. **/
        GLuint _vao;
        /** Associated vertex buffer. **/
        VertexBuffer* _vertexBuffer;
        /** Vertex attributes array. **/
        std::vector<Geometry::Attribute> _attributes;
};

} // Framework

#endif /* _DUMB_FW_VERTEX_STREAM_ */
