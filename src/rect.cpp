#include <DumbFramework/rect.hpp>

namespace Framework {

/** Default constructor. **/
Rect::Rect()
{}
/**
* Constructor.
* @param [in] p Upper left corner.
* @param [in] s Rectangle size.
*/
Rect::Rect(glm::vec2 const& p, glm::vec2 const& s)
    : position(p)
    , size(s)
{}
/**
* Copy constructor.
* @param [in] r Source rectangle.
*/
Rect::Rect(Rect const& r)
    : position(r.position)
    , size(r.size)
{}
/**
* Copy operator.
* @param [in] r Source rectangle.
*/
Rect& Rect::operator=(Rect const& r)
{
    position = r.position;
    size     = r.size;
    return *this;
}

} // Framework
