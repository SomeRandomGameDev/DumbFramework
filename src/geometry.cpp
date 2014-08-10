#include <DumbFramework/log.hpp>
#include <DumbFramework/geometry.hpp>

namespace Framework {
namespace Geometry  {

/** Default constructor. **/
Attribute::Attribute()
    : index(0)
    , type(ComponentType::FLOAT)
    , size(0)
    , stride(0)
    , offset(0)
{}
/** 
 * Constructor
 * @param [in] i   Index. 
 * @param [in] t   Components type.
 * @param [in] sz  Number of components.
 * @param [in] st  Stride.
 * @param [in] off Offset.  
 */
Attribute::Attribute(unsigned int i, ComponentType t, size_t sz, size_t st, size_t off)
    : index(i)
    , type(t)
    , size(sz)
    , stride(st)
    , offset(off)
{}
/**
 * Copy constructor.
 * @param [in] attr Input attribute.
 */
Attribute::Attribute(Attribute const& attr)
    : index(attr.index)
    , type(attr.type)
    , size(attr.size)
    , stride(attr.stride)
    , offset(attr.offset)
{}
/** 
 * Copy operator.
 * @param [in] attr Input attribute
 */
Attribute& Attribute::operator=(Attribute const& attr)
{
    index  = attr.index;
    type   = attr.type;
    size   = attr.size;
    stride = attr.stride;
    offset = attr.offset;
    return *this;
}

} // Geometry
} // Framework
