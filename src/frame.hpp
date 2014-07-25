#ifndef _DUMB_FW_FRAME_
#define _DUMB_FW_FRAME_

#include <glm/glm.hpp>

namespace Framework {

/**
 * An animation frame consists in a texture quad coordinate, an offset,
 * a size and a timestamp.
 * It is a non-mutable object.
 */
struct Frame
{
        double       time;    /**< Time of appearance in seconds. **/
        glm::ivec2   offset;  /**< Position offset. **/
        glm::ivec2   size;    /**< Size. **/
        glm::dvec2   top;     /**< Texture coordinate top (upper left). **/
        glm::dvec2   bottom;  /**< Texture coordinate bottom (lower right). **/
        unsigned int layer;   /**< Texture layer in the texture array. **/
};

} // Framework

#endif // _DUMB_FW_FRAME_
