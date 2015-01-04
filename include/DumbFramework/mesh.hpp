#ifndef _DUMB_FW_MESH_
#define _DUMB_FW_MESH_

#include <DumbFramework/geometry.hpp>
#include <DumbFramework/vertexbuffer.hpp>
#include <DumbFramework/indexbuffer.hpp>
#include <DumbFramework/boundingobjects.hpp>

namespace Framework {

/**
 * Simple mesh class.
 */
class Mesh
{
    // [todo] friend class MeshReader ?
    public:
        /**
         * Mesh attribute id.
         */
        enum AttributeId
        {
            Position = 0,
            Normal,
            // [todo] tangent, bitangent, bone id+weight...
            TexCoord,
            AttributeCount
        };
    
    public:
        /**
         * Default constructor.
         */
        Mesh();
        /**
         * Destructor.
         */
        ~Mesh();
        
        // [todo] create mesh ?
        
        /**
         * Release any resource allocated by this mesh.
         */
        void destroy();
        /**
         * Get geometry attribute.
         * @param [in] id Attribute identifier.
         * @return Geometry attribute. @warning It can be empty.
         */
        inline Geometry::Attribute const& attribute(AttributeId id) const;
        /**
         * Bind mesh vertex buffer.
         */
        inline void bind() const;
        /**
         * Unbind mesh vertex buffer and attributes.
         */
        inline void unbind() const;
        
        // [todo] get vertex, index count
        // [todo] draw command
        
        /**
         * Get mesh bounding sphere.
         * @return Bounding sphere.
         */
        inline BoundingSphere const& boundingSphere() const;
        /**
         * Get mesh axis aligned bounding box.
         * @return Axis aligned bounding box.
         */
        inline BoundingBox const& boundingBox() const;
        
    protected:
        // [todo] void computeVertexNormals();
        // [todo] void computeVertexTangents();
        /** Compute axis aligned bounding box and bounding sphere. **/
        void computeBoundingObjects();

    protected:
        /**
         * Vertex buffer.
         * This buffer contains:
         *    - position
         *    - texture coordinates
         *    - normals
         */
        VertexBuffer _vertexBuffer;
        /**
         * Index buffer.
         * This buffer contains indices to the vertex buffer for each
         * triangular face.
         */
        IndexBuffer  _indexBuffer;
        /**
         * Number of vertices.
         */
        size_t _vertexCount;
        /** Geometry attributes. **/
        Geometry::Attribute _attributes[AttributeCount];
        /** Bounding sphere. **/
        BoundingSphere _sphere;
        /** Axis aligned bounging sphere. **/
        BoundingBox    _aabb;
};

} // Framework

#include "mesh.inl"

#endif /* _DUMB_FW_MESH_ */
