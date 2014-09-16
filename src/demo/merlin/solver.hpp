#ifndef _MERLIN_SOLVER_
#define _MERLIN_SOLVER_

#include <cstdlib>
#include <cstdint>

#include <vector>

#include "board.hpp"

namespace Merlin {

/**
 * @brief Merlin/Lights out solver.
 * See "Turning Lights Out with Linear Algebra" by Marlow Anderson & Todd Feil
 * for more details.
 */
class Solver
{
    public:
        /**
         * Default constructor.
         */
        Solver();
        /**
         * Destructor.
         */
        ~Solver();
        /**
         * Create solver and build the necessary vectors/matrices.
         * @param [in] n Board size.
         * @return false if an error occured.
         */
        bool create(size_t n);
        /**
         * Destroy solver.
         */
        void destroy();
        /**
         * Check if the board is solvable.
         * @param [in] board Input board.
         * @return true if the board is solvable.
         */
        bool isSolvable(Board const& input) const;
         
    private:
        /**
         * Perform Gauss-Jordan reduction of the matrix of change
         * vectors (A).
         * This matrix represents the change implied to the board when
         * a given button is pressed.
         * At the end of the Gauss-Jordan reduction we will have, the 
         * inverse A (R), its rank and matrix A in the reduced-row
         * echelon form (E). We will only record the last two columns of
         * E (basis vectors.).
         * R is used to compute hints.
         * The basis vectors are used to check if a board is solvable. 
         */
        void gaussJordan();
        
    private:
        size_t   _size;    /**< Board size. **/
        size_t   _rank;    /**< Rank of the reduce-row echelon form matrix. **/
        uint8_t *_inverse; /**< Inverse of the matrix of change vectors. **/
        uint8_t *_hint;    /**< Cached hints for a given board. **/
        std::vector<uint8_t*> _basis; /**< Basis vectors.**/
};

} // Merlin

#endif // _MERLIN_SOLVER_
