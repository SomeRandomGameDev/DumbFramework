#include "solver.hpp"
#include <cstring>

namespace Merlin {

/**
 * Default constructor.
 */
Solver::Solver()
    : _size(0)
    , _rank(0)
    , _inverse(NULL)
    , _basis()
{}
/**
 * Destructor.
 */
Solver::~Solver()
{
    destroy();
}
/**
 * Create solver and build the necessary vectors/matrices.
 * @param [in] n Board size.
 * @return false if an error occured.
 */
bool Solver::create(size_t n)
{
    destroy();
    
    _size = n;
    
    gaussJordan();
    
    if(_basis.empty() || (NULL == _inverse))
    {
        return false;
    }
    return true;
}
/**
 * Destroy solver.
 */
void Solver::destroy()
{
    _size = 0;
    _rank = 0;
    if(NULL != _inverse)
    {
        delete [] _inverse;
        _inverse = NULL;
    }
    for(size_t i=0; i<_basis.size(); i++)
    {
        if(NULL != _basis[i])
        {
            delete [] _basis[i];
            _basis[i] = NULL;
        }
    }
    for(size_t i=0; i<_basis.size(); i++)
    {
        if(NULL != _basis[i])
        {
            delete [] _basis[i];
            _basis[i] = NULL;
        }
    }
    _basis.clear();
}
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
void Solver::gaussJordan()
{
    size_t s2 = _size * _size;
    uint8_t *toggle = new uint8_t[s2*s2];
    
    _inverse = new uint8_t[s2*s2];
   
    size_t i, j, k, offset;
    
    // Initialize to identity.
    memset(_inverse, 0, sizeof(uint8_t) * s2 * s2);
    for(j=0; j<s2; j++)
    {
        _inverse[j+(j*s2)] = 1;
    }
    
    // Initialize matrix of change vectors.
    memset(toggle, 0, sizeof(uint8_t) * s2 * s2);
    for(j=0; j<_size; j++)
    {
        for(i=0; i<_size; i++)
        {
            k = i + (j*_size);
            offset = k + (k*s2);
            
            toggle[offset] = 1;

            if(i > 0) { toggle[offset-1] = 1; }
            if(j > 0) { toggle[offset-_size] = 1; }
            if((i+1) < _size) { toggle[offset+1] = 1; }
            if((j+1) < _size) { toggle[offset+_size] = 1; }
        }
    }

    // Gauss-Jordan elimination.
    size_t pivot;
    for(k=0; k<s2; k++)
    {
        // First first non null element in current column.
        for(pivot=k; pivot<s2; pivot++)
        {
            if(toggle[k+(pivot*s2)]) break;
        }
        // Empty sub-column.
        if(pivot >= s2) break;
        
        // Swap rows if needed.
        if(pivot != k)
        {
            for(i=k; i<s2; i++)
            {
                uint8_t v = toggle[i+(k*s2)];
                toggle[i+(k*s2)] = toggle[i+(pivot*s2)]; 
                toggle[i+(pivot*s2)] = v;
            }
            // Don't forget to do the same for the inverse.
            for(i=0; i<s2; i++)
            {
                uint8_t v = _inverse[i+(k*s2)];
                _inverse[i+(k*s2)] = _inverse[i+(pivot*s2)]; 
                _inverse[i+(pivot*s2)] = v;
            }
        }
        // Perform elimination
        for(j=0; j<k; j++)
        {
            offset = k+(j*s2);
            for(i=0; i<s2; i++)
            {
                _inverse[i+(j*s2)] ^= _inverse[i+(k*s2)] & toggle[offset];
            }
            for(i=k+1; i<s2; i++)
            {
                toggle[i+(j*s2)] ^= toggle[i+(k*s2)] & toggle[offset];
            }
            toggle[offset] = 0;
        }
        for(j=k+1; j<s2; j++)
        {
            offset = k+(j*s2);
            for(i=0; i<s2; i++)
            {
                _inverse[i+(j*s2)] ^= _inverse[i+(k*s2)] & toggle[offset];
            }
            for(i=k+1; i<s2; i++)
            {
                toggle[i+(j*s2)] ^= toggle[i+(k*s2)] & toggle[offset];
            }
            toggle[offset] = 0;
        }
    }
    _rank = k;
    
    // Save the last 2 columns of the toggle matrix.
    if(_rank < s2)
    {
        size_t l = s2 - _rank;
        _basis.resize(l);
        for(k=0; k<l; k++)
        {
            _basis[k] = new uint8_t[s2];
            memset(&_basis[k][0], 0, s2);
            for(j=0; j<_rank; j++)
            {
                _basis[k][j] = toggle[_rank+k + (j*s2)];
            }
            _basis[k][k+_rank] = 1;
        }
    }
    
    delete [] toggle;
}
/**
 * Check if the board is solvable.
 * @param [in] input Input board.
 * @return true if the board is solvable.
 */
bool Solver::isSolvable(Board const& input) const
{
    if(_size != input._size)
    {
        return false;
    }
    
    for(size_t j=0; j<_basis.size(); j++)
    {
        uint8_t dot = 0;
        for(size_t i=0; i<_size; i++)
        {
            dot ^= _basis[j][i] & input._data[i];
        }
        if(dot) return false;
    }
    return true;
}

} // Merlin
