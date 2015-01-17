#include <DumbFramework/log.hpp>
#include <DumbFramework/geometry.hpp>

namespace Framework {
namespace Geometry  {

/** Default constructor. **/
Attribute::Attribute()
    : type(ComponentType::FLOAT)
    , size(0)
    , normalized(false)
    , stride(0)
    , offset(0)
    , divisor(0)
{}
/** 
 * Constructor
 * @param [in] t   Components type.
 * @param [in] sz  Number of components.
 * @param [in] n   Specify if the data should be normalized or not.
 * @param [in] st  Stride.
 * @param [in] off Offset.  
 * @param [in] d   Divisor (default=0).
 */
Attribute::Attribute(ComponentType t, size_t sz, bool n, size_t st, size_t off, unsigned int d)
    : type(t)
    , size(sz)
    , normalized(n)
    , stride(st)
    , offset(off)
    , divisor(d)
{}
/**
 * Copy constructor.
 * @param [in] attr Input attribute.
 */
Attribute::Attribute(Attribute const& attr)
    : type(attr.type)
    , size(attr.size)
    , normalized(attr.normalized)
    , stride(attr.stride)
    , offset(attr.offset)
    , divisor(attr.divisor)
{}
/** 
 * Copy operator.
 * @param [in] attr Input attribute
 */
Attribute& Attribute::operator=(Attribute const& attr)
{
    type       = attr.type;
    size       = attr.size;
    normalized = attr.normalized;
    stride     = attr.stride;
    offset     = attr.offset;
    divisor    = attr.divisor;
    return *this;
}

} // Geometry
} // Framework
