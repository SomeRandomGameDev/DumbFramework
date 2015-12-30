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
#ifndef _DUMB_FW_VERTEX_STREAM_
#define _DUMB_FW_VERTEX_STREAM_

#include <vector>
#include <initializer_list>

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/indexbuffer.hpp>
#include <DumbFramework/render/geometry.hpp>

namespace Dumb   {
namespace Render {

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
         * @return true if the vertex stream was succesfully created.
         */
        bool create();
        /**
         * Destroy vertex stream.
         */
        void destroy();
        /*
         * Add attribute to vertex stream.
         * @param [in] vertexBuffer Vertex buffer that will be associated
         *                          with the current attribute.
         * @param [in] index      Attribute index (starts at 0).
         * @param [in] type       Type of the attribute components.
         * @param [in] size       Number of components.
         * @param [in] normalize  Specify if the data should be normalized or not.
         * @param [in] stride     Number of bytes between 2 consecutives attributes.
         * @param [in] offset     Offset of the first attribute of the first vertex.
         * @param [in] divisor    Attribute divisor (default=0). 
         * @return true if the attribute was successfully set.
         */
        bool add(VertexBuffer const* vertexBuffer, unsigned int index, Geometry::ComponentType type, size_t size, bool normalize, size_t stride, size_t offset, unsigned int divisor=0);
        /**
         * Add attribute to vertex stream.
         * @param [in] vertexBuffer Vertex buffer that will be associated
         *                          with the current attribute.
         * @param [in] index  Attribute index.
         * @param [in] attr   Attribute description.
         * @return true if the attribute was successfully set.
         */
        bool add(VertexBuffer const* vertexBuffer, unsigned int index, Geometry::Attribute const& attr);
        /**
         * Add a set of attributes to vertex stream.
         * @param [in] vertexBuffer Vertex buffer that will be associated
         *                          with the current attributes.
         * @param [in] attr         A list of vertex attributes.
         * @return true if the attributes were successfully added.
         */
        bool add(VertexBuffer const* vertexBuffer, std::initializer_list< std::pair<unsigned int, Geometry::Attribute> > const& attr);
        /**
         * Add index buffer to vertex stream.
         */
        bool add(IndexBuffer* indexBuffer);
        /**
         * Check vertex attributes and build internal structures for
         * future rendering.
         * @return true if everything went right.
         */
        bool compile();
        /**
         * Bind vertex stream.
         */
        void bind() const;
        /**
         * Unbind vertex stream.
         */
        void unbind() const;
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
         * Draw primitives.
         * @note The vertex stream must have been bound.
         * @param [in] primitive Primitive type to be drawn.
         * @param [in] offset    Start offset in the vertex buffer.
         * @param [in] count     Number of primitives to draw.
         */
        void draw(Geometry::Primitive primitive, size_t offset, size_t count) const;
        /**
         * Check if the attribute at the specified index was set.
         * @param [in] index Attribute index.
         * @return true if the attribute is set.
         */
        bool isAttributeSet(unsigned int index) const;
        /**
         * Get the vertex buffer for a given attribute.
         * @param [in] index Attribute index.
         * @return Constant pointer to the vertex buffer.
         */
        VertexBuffer const* getVertexBuffer(unsigned int index) const;
        /**
         * Get index buffer.
         */
        IndexBuffer const* getIndexBuffer() const;
    private:
        /** Vertex array identifier. **/
        GLuint _vao;
        /** Vertex attributes array. **/
        std::vector<std::pair<const VertexBuffer*, Geometry::Attribute>> _attributes;
        /** Index buffer. **/
        IndexBuffer *_elements;
};

} // Render
} // Dumb

#endif /* _DUMB_FW_VERTEX_STREAM_ */
