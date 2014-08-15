#ifndef _DUMB_FW_GEOMETRY_
#define _DUMB_FW_GEOMETRY_

#include <GL/glew.h>
#include <DumbFramework/config.hpp>

namespace Framework {
/**
 * @defgroup DUMB_FW_GEOMETRY_RENDERING Geometry rendering.
 * @ingroup DUMB_FW_RENDERING
 * GPU based geometry rendering.
 */
namespace Geometry  {
/**
 * Supported rendering primitives.
 * @ingroup DUMB_FW_GEOMETRY_RENDERING
 */
struct Primitive
{
    /** Primitive types. **/
    enum Value
    {
        /** 
         * A list of disjoints triangles. A triangle is 
         * generated every 3 vertices. Vertices \f$n\f$, 
         * \f$n+1\f$ and \f$n+2\f$ will be used to generated
         * a triangle. The next one will be using vertices 
         * \f$n+3\f$, \f$n+4\f$ and \f$n+5\f$. 
         */
        TRIANGLES,
        /** 
         * A list of contiguous triangles. Each triangle shares
         * 2 vertices with its predecessor. A triangle is
         * generated using the vertices \f$n-2\f$, \f$n-1\f$ and
         * \f$n\f$. The next triangle will be using vertices
         * \f$n-1\f$, \f$n\f$ and \f$n+1\f$. With the exception
         * of the first 2 vertices, each new vertex in the buffer 
         * generates a triangle.
         */
        TRIANGLE_STRIP,
        /**
         * A list of contiguous triangles. All triangles share
         * the same vertex. This vertex is the first element of
         * the buffer. A triangle is generated using the 
         * vertices \f$0\f$, \f$n-1\f$, \f$n\f$. The next one
         * \f$0\f$, \f$n\f$, \f$n+1\f$. With the exception of 
         * the first 2 vertices, each new vertex in the buffer 
         * generates a triangle.
         */
        TRIANGLE_FAN,
        /** 
         * A list of disjoints lines. A line is generated every
         * 2 vertices. This means that a line will be generated
         * using vertices \f$n\f$ and \f$n+1\f$, and the next
         * one with vertices \f$n+2\f$ and \f$n+3\f$.
         */
        LINES,
        /** 
         * A list of contiguous lines. Each line will be
         * generated using the last point of the previous line.
         * This means that a line will be generated using
         * vertices \f$n-1\f$ and \f$n\f$ and the next one with
         * vertices \f$n\f$ \f$n+1\f$. So each time a new vertex
         * in the buffer will generate a new line.
         */
        LINE_STRIP,
        /**
         * A list of points.
         */
        POINTS
    };
    /** Primitive type. **/
    Value value;
    /** 
     * @brief Default constructor.
     * By default the primitive type is @c Primitive::TRIANGLES.
     */
    inline Primitive() : value(TRIANGLES) {}
    /**
     * @brief Constructor.
     * @param [in] v  Primitive type.
     */
    inline Primitive(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** Convert to OpenGL compliant value. **/
    inline operator GLenum();
};

/**
 * Vertex attribute component type.
 * @ingroup DUMB_FW_GEOMETRY_RENDERING
 */
struct ComponentType
{
    /** Component types. **/
    enum Value
    {
        /**
         * 8 bytes signed integer. 
         */
        BYTE,
        /** 8 bytes unsigned integer. **/
        UNSIGNED_BYTE,
        /** 16 bytes signed integer. **/
        SHORT,
        /** 16 bytes unsigned integer. **/
        UNSIGNED_SHORT,
        /** 32 bytes signed integer. **/
        INT,
        /** 32 bytes unsigned integer. **/
        UNSIGNED_INT,
        /** 16 bytes floating point. **/
        HALF_FLOAT,
        /** 32 bytes floating point. **/
        FLOAT,
        /** 64 bytes floating point. **/
        DOUBLE
    };
    /** Component type. **/
    Value value;
    /** 
     * @brief Default constructor.
     * By default the component type is @c ComponentType::FLOAT.
     */
    inline ComponentType() : value(FLOAT) {}
    /**
     * Constructor.
     * @param [in] v Component type.
     */
    inline ComponentType(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** Convert to OpenGL compliant value. **/
    inline operator GLenum();
};

/**
 * Vertex attribute.
 * Defines a vertex attribute in a vertex buffer.
 * @ingroup DUMB_FW_GEOMETRY_RENDERING
 */
struct Attribute
{
    /**
     * Attribute index.
     */
    unsigned int index;
    /**
     * Type of the attribute components.
     */
    ComponentType type;
    /**
     * Number of components.
     */
    size_t size;
    /**
     * Number of bytes between 2 consecutives attributes.
     */
    size_t stride;
    /**
     * Offset of the first attribute of the first vertex.
     */
    size_t offset;
    /**
     * Divisior...
     */
    unsigned int divisor;
    
    /** Default constructor. **/
    Attribute();
    /** 
     * Constructor 
     * @param [in] i   Index.
     * @param [in] t   Components type.
     * @param [in] sz  Number of components.
     * @param [in] st  Stride.
     * @param [in] off Offset.
     * @param [in] d   Divisor (default=0).  
     */
    Attribute(unsigned int index, ComponentType t, size_t sz, size_t st, size_t off, unsigned int d=0); 
    /**
     * Copy constructor.
     * @param [in] attr Input attribute.
     */
    Attribute(Attribute const& attr);
    /** Destructor **/
    ~Attribute() {}
    /** 
     * Copy operator.
     * @param [in] attr Input attribute
     */
    Attribute& operator=(Attribute const& attr);
};

} // Geometry
} // Framework

#include "geometry.inl"

#endif /* _DUMB_FW_GEOMETRY_ */
