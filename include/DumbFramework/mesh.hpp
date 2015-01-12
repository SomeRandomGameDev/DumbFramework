#ifndef _DUMB_FW_MESH_
#define _DUMB_FW_MESH_

#include <DumbFramework/geometry.hpp>
#include <DumbFramework/vertexbuffer.hpp>
#include <DumbFramework/indexbuffer.hpp>
#include <DumbFramework/boundingobjects.hpp>

namespace Framework {
namespace Render    {

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
        /**
         * Mesh attribute bit mask.
         */
        enum AttributeMask
        {
            HasPosition = (1 << Position),
            HasNormal   = (1 << Normal),
            // [todo] HasTangent, HasBitangent ....
            HasTexCoord = (1 << TexCoord)
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
        /**
         * Create mesh.
         * @param [in] vertexCount   Vertex count.
         * @param [in] triangleCount Triangle count.
         * @param [in] mask          Attribute masks.
         * @return false if an error occured.
         */
        bool create(size_t vertexCount, size_t triangleCount, AttributeMask mask);
        /**
         * Release any resource allocated by this mesh.
         */
        void destroy();
        /**
         * Bind mesh vertex buffer.
         */
        inline void bind() const;
        /**
         * Unbind mesh vertex buffer and attributes.
         */
        inline void unbind() const;
        /**
         * Get geometry attribute.
         * @param [in] id Attribute identifier.
         * @return Geometry attribute. @warning It can be empty.
         */
        Geometry::Attribute const& attribute(AttributeId id) const;
        /**
         * Get vertex count.
         */
        inline size_t vertexCount() const;
        /**
         * Get vertex size in bytes.
         */
        inline size_t vertexSize() const;
        /**
         * Get index count.
         */
        inline size_t triangleCount() const;
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
        
        /** Number of vertices. **/
        size_t _vertexCount;
        /** triangle count. **/
        size_t _triangleCount;
        /** Primitive type. **/
        Geometry::Primitive _primitiveType;
        
        /** Geometry attributes. **/
        Geometry::Attribute _attributes[AttributeCount];
        /** Bounding sphere. **/
        BoundingSphere _sphere;
        /** Axis aligned bounging sphere. **/
        BoundingBox    _aabb;
};

} // Render
} // Framework

#include "mesh.inl"

#endif /* _DUMB_FW_MESH_ */
