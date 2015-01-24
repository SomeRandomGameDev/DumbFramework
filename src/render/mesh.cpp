#include <cstring>
#include <DumbFramework/render/mesh.hpp>

namespace Framework {
namespace Render    {

/* Vertex attributes. */
static const Geometry::Attribute vertexAttributes[Mesh::AttributeCount] = 
{
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, 0, 0, 0), // Position
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 2, false, 0, 0, 0), // TexCoord
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, 0, 0, 0), // Normal
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 4, false, 0, 0, 0)  // Tangent
};

/**
 * Default constructor.
 */
Mesh::Mesh()
    : _vertexBuffer()
    , _indexBuffer()
    , _vertexCount(0)
    , _triangleCount(0)
    , _primitiveType(Geometry::Primitive::TRIANGLES)
    , _attributesMask(Mesh::None)
    , _sphere()
    , _aabb()
{}
/**
 * Destructor.
 */
Mesh::~Mesh()
{}
/**
 * Create mesh.
 * @param [in] vertexCount   Vertex count.
 * @param [in] triangleCount Triangle count.
 * @param [in] mask          Attribute masks.
 * @param [in] vertexData    Pointer to vertex data (nullptr by default).
 * @param [in] indexData     Pointer to triangle data (nullptr by default).
 * @return false if an error occured.
 */
bool Mesh::create(size_t vertexCount, size_t triangleCount, uint32_t mask, void* vertexData, void* indexData)
{
    bool ret;
    
    _triangleCount  = triangleCount;
    _vertexCount    = vertexCount;
    _primitiveType  = Geometry::Primitive::TRIANGLES;
    _attributesMask = mask;
    
    // stride will be fixed later.
    size_t offset = 0;
    for(size_t i=0; i<AttributeCount; i++)
    {
        if(mask & (1 << i))
        {
            memcpy(&_attributes[i], &vertexAttributes[i], sizeof(Geometry::Attribute));
            _attributes[i].offset = offset;
            offset += _attributes[i].bytes();
        }
    }

    // fix stride.
    size_t stride = offset;
    for(size_t i=0; i<AttributeCount; i++)
    {
        _attributes[i].stride = stride;
    }
    
    ret = _indexBuffer.create(3 * triangleCount * sizeof(GLuint), indexData, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create index buffer!");
        return false;
    }
    
    ret = _vertexBuffer.create(vertexCount * stride, vertexData, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create vertex buffer!");
        return false;
    }
    
    if(nullptr != vertexData)
    {
        update();
    }
    return true;
}
/**
 * Set vertex attribute data.
 * @param [in] id  Attribute id.
 * @param [in] ptr Pointer to vertex attribute data.
 * @return false if the attribute is not supported by the mesh or if the 
 *         pointer is null.
 */
bool Mesh::setAttribute(AttributeId id, uint8_t* ptr)
{
    if(nullptr == ptr)
    {
        Log_Error(Module::Render, "Invalid data pointer.");
        return false;
    }
    if(0 == (_attributesMask & (1 << id)))
    {
        Log_Error(Module::Render, "Unsupported vertex attribute.");
        return false;
    }
    
    uint8_t* data = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::WRITE_ONLY);
    data += _attributes[id].offset;
    size_t size   = _attributes[id].bytes();
    size_t stride = _attributes[id].stride;
    for(size_t i=0; i<_vertexCount; i++)
    {
        memcpy(data, ptr, size);
        ptr  += size;
        data += stride;
    }
    return _vertexBuffer.unmap();
}
/**
 * Update mesh.
 * It recomputes bounding box and bounding sphere, as long as
 * vertex normals, tangents and bitangents.
 */
void Mesh::update()
{
    computeBoundingObjects();
    // [todo]
    computeTangents();
    // [todo] compute normals, tangents and bitangents.
}
/**
 * Release any resource allocated by this mesh.
 */
void Mesh::destroy()
{
    _vertexBuffer.destroy();
    _indexBuffer.destroy();
}
/** Compute vertex tangents. **/
void Mesh::computeTangents()
{
    const uint8_t* vertex = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::READ_WRITE);
    if(nullptr == vertex)
    {
        return;
    }

    const GLuint* face = (GLuint*)_indexBuffer.map(BufferObject::Access::Policy::READ_ONLY);
    if(nullptr == face)
    {
        return;
    }
    size_t delta;
    float* ptr;
    
    glm::vec3* tangent[2];
    tangent[0] = new glm::vec3[2*_vertexCount];
    tangent[1] = tangent[0] + _vertexCount;
    memset(tangent[0], 0, sizeof(glm::vec3) * 2 * _vertexCount);

    for(size_t i=0; i<_triangleCount; i++)
    {
        glm::vec3 position[3];
        glm::vec2 texcoord[3];
        
        for(size_t j=0; j<3; j++)
        {
            delta = _attributes[Position].offset + (face[j] * _attributes[Position].stride);
            ptr = (float*)(vertex + delta);
            position[j].x = ptr[0];
            position[j].y = ptr[1];
            position[j].z = ptr[2];
            
            delta = _attributes[TexCoord].offset + (face[j] * _attributes[TexCoord].stride);
            ptr = (float*)(vertex + delta);
            texcoord[j].x = ptr[0];
            texcoord[j].y = ptr[1];
        }
        
        glm::vec3 e0 = position[1] - position[0];
        glm::vec3 e1 = position[2] - position[0];
        glm::vec2 t0 = texcoord[1] - texcoord[0];
        glm::vec2 t1 = texcoord[2] - texcoord[0];
        
        float r = 1.0f / (t0.x*t1.y - t0.y*t1.x);
        glm::vec3 sdir = r * (t1.y*e0 - t0.y*e1);
        glm::vec3 tdir = r * (t0.x*e1 - t1.x*e0);
        
        for(size_t j=0; j<3; j++)
        {
            tangent[0][face[j]] += sdir;
            tangent[1][face[j]] += tdir;
        }
        face += 3;
    }
    _indexBuffer.unmap();
    
    for(size_t i=0; i<_vertexCount; i++)
    {
        delta = _attributes[Normal].offset + (i * _attributes[Normal].stride);
        ptr = (float*)(vertex + delta);
        glm::vec3 n(ptr[0], ptr[1], ptr[2]);
        
        delta = _attributes[Tangent].offset + (i * _attributes[Tangent].stride);
        ptr = (float*)(vertex + delta);
        
        glm::vec3 t(tangent[0][i]);
        
        glm::vec3 ft = glm::normalize(t - n*glm::dot(n,t));
        ptr[0] = ft.x;
        ptr[1] = ft.y;
        ptr[2] = ft.z;
        ptr[3] = (glm::dot(glm::cross(n,t), tangent[1][i]) < 0.0f) ? -1.0f : 1.0f;
    }
    _vertexBuffer.unmap();

    delete [] tangent[0];
}
/** Compute axis aligned bounding box and bounding sphere. **/
void Mesh::computeBoundingObjects()
{
    const uint8_t *ptr = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::READ_ONLY);
    if(nullptr == ptr)
    {
        return;
    }
    _aabb = BoundingBox(ptr+_attributes[Position].offset, _vertexCount, _attributes[Position].stride);
    _vertexBuffer.unmap();
    
    _sphere = BoundingSphere(_aabb.getCenter(), glm::distance(_aabb.getMin(), _aabb.getMax()) / 2.0f);
}

} // Render
} // Framework


