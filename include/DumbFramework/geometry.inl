namespace Framework {
namespace Geometry  {
    
Primitive::operator GLenum()
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

ComponentType::operator GLenum()
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

} // Geometry
} // Framework
