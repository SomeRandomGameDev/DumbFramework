#ifndef _DUMB_FW_MESH_
#define _DUMB_FW_MESH_

#include <DumbFramework/geometry.hpp>
#include <DumbFramework/boundingobjects.hpp>

namespace Framework {
    
class Mesh
{
    // [todo] friend class MeshReader ?
    public:
        enum AttributeId
        {
            Position = 0,
            Normal,
            // [todo] tangent, bitangent, bone id+weight...
            TexCoord,
            AttributeCount
        };
    
    public:
        Mesh();
        ~Mesh();
        
        // [todo] create mesh ?
        
        void destroy();
        
        inline Geometry::Attribute const& attribute(AttributeId id) const;
        
        void bind(size_t startOffset) const;
        void unbind() const;
        
        inline BoundingSphere const& boundingSphere() const;
        inline BoundingBox const& boundingBox() const;
        
    protected:
        void computeVertexNormals();
        void computeVertexTangents();
        void computeAABB();
        void computeBoundingSphere();
    
    protected:
        VertexBuffer _vertexBuffer;
        IndexBuffer  _indexBuffer;
        
        Geometry::Attribute _geometryAttributes[AttributeCount];
        
        BoundingSphere _sphere;
        BoundingBox    _aabb;
};
    
} // Framework

#endif /* _DUMB_FW_MESH_ */
