#include <DumbFramework/log.hpp>
namespace Framework {
namespace Render    {
namespace Geometry  {
    
GLenum Primitive::to() const
{
    switch(value)
    {
        case Primitive::TRIANGLES:
            return GL_TRIANGLES;
        case Primitive::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case Primitive::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case Primitive::LINES:
            return GL_LINES;
        case Primitive::LINE_STRIP:
            return GL_LINE_STRIP;
        case Primitive::POINTS:
            return GL_POINTS;
        default:
            return GL_TRIANGLES;
    }
}

GLenum ComponentType::to() const
{
    switch(value)
    {
        case ComponentType::BYTE:
            return GL_BYTE;
        case ComponentType::UNSIGNED_BYTE:
            return GL_UNSIGNED_BYTE;
        case ComponentType::SHORT:
            return GL_SHORT;
        case ComponentType::UNSIGNED_SHORT:
            return GL_UNSIGNED_SHORT;
        case ComponentType::INT:
            return GL_INT;
        case ComponentType::UNSIGNED_INT:
            return GL_UNSIGNED_INT;
        case ComponentType::HALF_FLOAT:
            return GL_HALF_FLOAT;
        case ComponentType::FLOAT:
            return GL_FLOAT;
        case ComponentType::DOUBLE:
            return GL_DOUBLE;
        default:
            return GL_FLOAT;
    }
}

/** Size in bytes. **/
size_t ComponentType::size() const
{
    switch(value)
    {
        case ComponentType::BYTE:
            return sizeof(GLbyte);
        case ComponentType::UNSIGNED_BYTE:
            return sizeof(GLubyte);
        case ComponentType::SHORT:
            return sizeof(GLshort);
        case ComponentType::UNSIGNED_SHORT:
            return sizeof(GLushort);
        case ComponentType::INT:
            return sizeof(GLint);
        case ComponentType::UNSIGNED_INT:
            return sizeof(GLuint);
        case ComponentType::HALF_FLOAT:
            return sizeof(GLhalf);
        case ComponentType::FLOAT:
            return sizeof(GLfloat);
        case ComponentType::DOUBLE:
            return sizeof(GLdouble);
        default:
            return sizeof(GLfloat);
    }
}

/**
 * Get attribute size in bytes.
 */
size_t Attribute::bytes() const
{
    return size * type.size();
}

/**
 * Attach attribute.
 * @param [in] index Attribute index.
 */
void Attribute::attach(int index) const
{
    glVertexAttribPointer(index, size, type.to(), normalized ? GL_TRUE : GL_FALSE, stride, (const GLvoid*)offset);
    glVertexAttribDivisor(index, divisor);
}

} // Geometry
} // Render
} // Framework
