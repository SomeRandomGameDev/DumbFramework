#ifndef _MERLIN_BOARD_
#define _MERLIN_BOARD_

#include <stdint.h>
#include <glm/glm.hpp>

namespace Merlin {

/**
 * @brief Merlin/Lights out board.
 * This is basically a N*N bit matrix.
 */
class Board
{
    friend class Solver;
    
    public:
        /**
         * Default constructor.
         */
        Board();
        /**
         * Destructor.
         */
        ~Board();
        
        /**
         * Create n*n board.
         * @param [in] n  Board dimension.
         * @param [in] in Board data.
         * @return true upon success.
         */
        bool create(size_t n, uint8_t const* in=NULL);
        /**
         * Destroy board.
         */
        void destroy();
        /**
         * Get board size.
         */
        size_t size() const;
        /**
         * Get board value at a given coordinate.
         * @param [in] pos Cell coordinate.
         * @return Board value at pos or 0xff if the position was out
         *         of bounds.
         */
        uint8_t get(glm::ivec2 const& pos) const;
        /**
         * "Push" a board cell.
         * When a cell is pushed, its vertical and horizontal neighbours
         * are switch on or off depending of their original states.
         * @param [in] pos Cell position.
         * @return true if all the cells are off.
         */
        bool press(glm::ivec2 const& pos);
        /**
         * Return the number of lit cells.
         */
        unsigned int litCount() const;
        
    private:
        uint8_t *_data; /**< Cells. **/
        size_t   _size; /**< Size.  **/
};

}; // Merlin

#endif // _MERLIN_BOARD_
