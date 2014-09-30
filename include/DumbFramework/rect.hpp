#ifndef _DUMB_FW_BOUNDING_QUAD_
#define _DUMB_FW_BOUNDING_QUAD_

#include <glm/glm.hpp>

namespace Framework {

/**
 * Simple rectangle.
 */
class Rect
{
    public:
        /** Default constructor. **/
        Rect();
        /**
         * Constructor.
         * @param [in] p Upper left corner.
         * @param [in] s Rectangle size.
         */
        Rect(glm::vec2 const& position, glm::vec2 const& size);
        /**
         * Copy constructor.
         * @param [in] r Source rectangle.
         */
        Rect(Rect const& r);
        /**
         * Copy operator.
         * @param [in] r Source rectangle.
         */
        Rect& operator=(Rect const& r);
        /**
         * Compute minimum point.
         */
        inline glm::vec2 min() const;
        /**
         * Compute maximum point.
         */
        inline glm::vec2 max() const;
        /**
         * Compute center.
         */
        inline glm::vec2 center() const;
        /**
         * Compute extent.
         */
        inline glm::vec2 extent() const;
        
    public:
        /** Upper left corner. **/
        glm::vec2 position;
        /** Size. **/
        glm::vec2 size;
};

} // Framework

#include "rect.inl"

#endif // _DUMB_FW_BOUNDING_QUAD_
