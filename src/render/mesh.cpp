#include <cstring>
#include <DumbFramework/render/mesh.hpp>

namespace Framework {
namespace Render    {

/** Vertex size in bytes. */
const size_t Mesh::vertexSize = 12*sizeof(float);
/** Geometry attributes. **/
const Geometry::Attribute Mesh::attributes[Mesh::AttributeCount] = 
{
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, vertexSize, 0              , 0), // Position
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 2, false, vertexSize, 3*sizeof(float), 0), // TexCoord
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, vertexSize, 5*sizeof(float), 0), // Normal
    Geometry::Attribute(Geometry::ComponentType::FLOAT, 4, false, vertexSize, 8*sizeof(float), 0)  // Tangent
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
    
    ret = _indexBuffer.create(3 * triangleCount * sizeof(GLuint), indexData, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create index buffer!");
        return false;
    }
    
    ret = _vertexBuffer.create(vertexCount * vertexSize, vertexData, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create vertex buffer!");
        return false;
    }

    float* src = (float*)vertexData;
    _vertexBuffer.bind();
    uint8_t* data = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::WRITE_ONLY);
    for(int i=0; i<AttributeCount; i++)
    {
        uint8_t* dest = data + attributes[i].offset;
        if(mask & (1<<i))
        {
            for(size_t j=0; j<vertexCount; j++)
            {
                memcpy(dest, src, attributes[i].bytes());
                src  += attributes[i].size;
                dest += attributes[i].stride;
            }
        }
    }
    _vertexBuffer.unmap();
    _vertexBuffer.unbind();
    
    update(mask);
    return createVertexStream();
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
    bool ret;
    _vertexBuffer.bind();
    uint8_t* data = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::WRITE_ONLY);
    data += attributes[id].offset;
    size_t size   = attributes[id].bytes();
    size_t stride = attributes[id].stride;
    for(size_t i=0; i<_vertexCount; i++)
    {
        memcpy(data, ptr, size);
        ptr  += size;
        data += stride;
    }
    ret = _vertexBuffer.unmap();
    _vertexBuffer.unbind();
    return ret;
}
/**
 * Update mesh.
 * It recomputes bounding box and bounding sphere, as long as
 * vertex normals, tangents and bitangents.
 */
void Mesh::update(uint32_t mask)
{
    computeBoundingObjects();
    if(0 == (mask & Mesh::HasNormal))
    {
        computeNormals();
    }
    if(0 == (mask & Mesh::HasTangent))
    {
        computeTangents();
    }
}
/**
 * Release any resource allocated by this mesh.
 */
void Mesh::destroy()
{
    _vertexBuffer.destroy();
    _indexBuffer.destroy();
}
/** Compute vertex normals. **/
void Mesh::computeNormals()
{
    _vertexBuffer.bind();
        const uint8_t* vertex = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::READ_WRITE);
    _vertexBuffer.unbind();
    if(nullptr == vertex)
    {
        return;
    }

    _indexBuffer.bind();
        const GLuint* face = (GLuint*)_indexBuffer.map(BufferObject::Access::Policy::READ_ONLY);
    _indexBuffer.unbind();
    if(nullptr == face)
    {
        return;
    }

    float *ptr;
    for(size_t i=0; i<_vertexCount; i++)
    {
        ptr = (float*)(vertex + attributes[Normal].offset + (i * attributes[Normal].stride));
        memset(ptr, 0, attributes[Normal].bytes());
    }

    for(size_t i=0; i<_triangleCount; i++)
    {
        glm::vec3 n;
        glm::vec3 p[3];
        
        for(size_t j=0; j<3; j++)
        {
            ptr = (float*)(vertex + attributes[Position].offset + (face[j] * attributes[Position].stride));
            p[j].x = ptr[0];
            p[j].y = ptr[1];
            p[j].z = ptr[2];
        }
        
        n = glm::normalize(glm::cross(p[1]-p[0], p[2]-p[0]));
        
        for(size_t j=0; j<3; j++)
        {
            ptr = (float*)(vertex + attributes[Normal].offset + (face[j] * attributes[Normal].stride));
            ptr[0] += n.x;
            ptr[1] += n.y;
            ptr[2] += n.z;
        }
        face += 3;
    }

    for(size_t i=0; i<_vertexCount; i++)
    {
        ptr = (float*)(vertex + attributes[Normal].offset + (i * attributes[Normal].stride));
        glm::vec3 n = glm::normalize(glm::vec3(ptr[0], ptr[1], ptr[2]));
        ptr[0] = n.x;
        ptr[1] = n.y;
        ptr[2] = n.z;
    }
    _indexBuffer.bind();
        _indexBuffer.unmap();
    _indexBuffer.unbind();
    
    _vertexBuffer.bind();
        _vertexBuffer.unmap();
    _vertexBuffer.unbind();
}

/** Compute vertex tangents. **/
void Mesh::computeTangents()
{
    _vertexBuffer.bind();
        const uint8_t* vertex = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::READ_WRITE);
    _vertexBuffer.unbind();
    if(nullptr == vertex)
    {
        return;
    }

    _indexBuffer.bind();
        const GLuint* face = (GLuint*)_indexBuffer.map(BufferObject::Access::Policy::READ_ONLY);
    _indexBuffer.unbind();
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
            delta = attributes[Position].offset + (face[j] * attributes[Position].stride);
            ptr = (float*)(vertex + delta);
            position[j].x = ptr[0];
            position[j].y = ptr[1];
            position[j].z = ptr[2];
            
            delta = attributes[TexCoord].offset + (face[j] * attributes[TexCoord].stride);
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
    _indexBuffer.bind();
        _indexBuffer.unmap();
    _indexBuffer.unbind();
    
    for(size_t i=0; i<_vertexCount; i++)
    {
        delta = attributes[Normal].offset + (i * attributes[Normal].stride);
        ptr = (float*)(vertex + delta);
        glm::vec3 n(ptr[0], ptr[1], ptr[2]);
        
        delta = attributes[Tangent].offset + (i * attributes[Tangent].stride);
        ptr = (float*)(vertex + delta);
        
        glm::vec3 t(tangent[0][i]);
        
        glm::vec3 ft = glm::normalize(t - n*glm::dot(n,t));
        ptr[0] = ft.x;
        ptr[1] = ft.y;
        ptr[2] = ft.z;
        ptr[3] = (glm::dot(glm::cross(n,t), tangent[1][i]) < 0.0f) ? -1.0f : 1.0f;
    }
    _vertexBuffer.bind();
        _vertexBuffer.unmap();
    _vertexBuffer.unbind();

    delete [] tangent[0];
}
/** Compute axis aligned bounding box and bounding sphere. **/
void Mesh::computeBoundingObjects()
{
    _vertexBuffer.bind();
        const uint8_t *ptr = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::READ_ONLY);
    _vertexBuffer.unbind();
    if(nullptr == ptr)
    {
        return;
    }
    _aabb = Dumb::Core::Geometry::BoundingBox(ptr+attributes[Position].offset, _vertexCount, attributes[Position].stride);
    _vertexBuffer.bind();
        _vertexBuffer.unmap();
    _vertexBuffer.unbind();

    _sphere = Dumb::Core::Geometry::BoundingSphere(_aabb.getCenter(), glm::distance(_aabb.getMin(), _aabb.getMax()) / 2.0f);
}
/** Create vertex stream. **/
bool Mesh::createVertexStream()
{
    bool ret;
    _stream.create();
    for(size_t i=0; i<Mesh::AttributeCount; i++)
    {
        ret = _stream.add(&_vertexBuffer, i, attributes[i]);
        if(false == ret)
        {
            Log_Error(Module::Render, "Failed to add attribute %d to stream.", i);
            return false;
        }
    }
    _stream.add(&_indexBuffer);
    
    ret = _stream.compile();
    if(false == ret)
    {
        Log_Error(Module::Render, "Stream compilation failed.");
        return false;
    }
    return true;
}

} // Render
} // Framework


