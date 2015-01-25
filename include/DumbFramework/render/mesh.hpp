#ifndef _DUMB_FW_MESH_
#define _DUMB_FW_MESH_

#include <DumbFramework/boundingobjects.hpp>
#include <DumbFramework/render/geometry.hpp>
#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/indexbuffer.hpp>

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
            TexCoord,
            Normal,
            Tangent,
            // [todo] bone id + weight ...
            AttributeCount
        };
        /**
         * Mesh attribute bit mask.
         */
        enum AttributeMask
        {
            None         = 0,
            HasPosition  = (1 << Position),
            HasTexCoord  = (1 << TexCoord),
            HasNormal    = (1 << Normal),
            HasTangent   = (1 << Tangent),
            // [todo] bone id + weight ...
            HasAll       = (1 << AttributeCount) - 1
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
         * @param [in] vertexData    Pointer to vertex data (nullptr by default).
         * @param [in] indexData     Pointer to triangle data (nullptr by default).
         * @return false if an error occured.
         */
        bool create(size_t vertexCount, size_t triangleCount, uint32_t mask, void* vertexData=nullptr, void* indexData=nullptr);
        /**
         * Set vertex attribute data.
         * @param [in] id  Attribute id.
         * @param [in] ptr Pointer to vertex attribute data.
         * @return false if the attribute is not supported by the mesh or if the 
         *         pointer is null.
         */
        bool setAttribute(AttributeId id, uint8_t* ptr);
        // [todo] setAttributes
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
         * Get vertex buffer.
         */
        inline VertexBuffer const& vertexBuffer() const;
        /**
         * Get indeex buffer.
         */
        inline IndexBuffer const& indexBuffer() const;
        /**
         * Get vertex count.
         */
        inline size_t vertexCount() const;
        /**
         * Get index count.
         */
        inline size_t triangleCount() const;
        
        // [todo] map vertex buffer
        // [todo] map index buffer
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
    public:
        /** Vertex size in bytes. */
        static const size_t vertexSize;
        /** Geometry attributes. **/
        static const Geometry::Attribute attributes[AttributeCount];
        
    protected:
        /**
         * Update mesh.
         * It recomputes bounding box and bounding sphere, as long as
         * vertex normals, tangents and bitangents.
         */
        void update(uint32_t mask);
        /** Compute vertex normals. **/
        void computeNormals();
        /** Compute vertex tangents. **/
        void computeTangents();
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
        /** Bounding sphere. **/
        BoundingSphere _sphere;
        /** Axis aligned bounging sphere. **/
        BoundingBox    _aabb;
};

} // Render
} // Framework

#include "mesh.inl"

#endif /* _DUMB_FW_MESH_ */
