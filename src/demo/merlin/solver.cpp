#include "solver.hpp"
#include <cstring>

#include <stdio.h>

namespace Merlin {

/**
 * Default constructor.
 */
Solver::Solver()
    : _size(0)
    , _rank(0)
    , _inverse(NULL)
    , _hint(NULL)
{
    _basis[0] = _basis[1] = NULL;
}
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
    _hint = new uint8_t[_size*_size];
    if(NULL == _hint)
    {
        return false;
    }
    
    gaussJordan();
    
    if((NULL == _basis) || (NULL == _inverse))
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
    for(size_t i=0; i<2; i++)
    {
        if(NULL != _basis[i])
        {
            delete [] _basis[i];
            _basis[i] = NULL;
        }
    }
    if(NULL != _hint)
    {
        delete [] _hint;
        _hint = NULL;
    }
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
    uint8_t *a = new uint8_t[s2*s2];
    
    _inverse = new uint8_t[s2*s2];
    _basis[0] = new uint8_t[s2];
    _basis[1] = new uint8_t[s2];

    size_t i, j, k;
    
    // Initialize to identity.
    memset(_inverse, 0, sizeof(uint8_t) * s2 * s2);
    for(j=0; j<s2; j++)
    {
        _inverse[j+(j*s2)] = 1;
    }
    
    // Initialize matrix of change vectors.
    memset(a, 0, sizeof(uint8_t) * s2 * s2);
    for(k=0; k<_size; k++)
    {
        uint8_t *m = a + (k*_size) + ((k*_size) * s2);
        for(j=0; j<_size; j++)
        {
            if(j > 0)
                m[(j-1) + (j*s2)] = 1;
            if(j < (_size-1))
                m[(j+1) + (j*s2)] = 1;
            m[(j ) + (j*s2)] = 1;
        }

        if(k > 0)
        {
            m = a + ((k-1)*_size) + ((k*_size) * s2); 
            for(j=0; j<_size; j++)
            {
                m[j + (j*s2)] = 1;
            }
        }
        
        if(k < (_size-1))
        {
            m = a + ((k+1)*_size) + ((k*_size) * s2); 
            for(size_t j=0; j<_size; j++)
            {
                m[j + (j*s2)] = 1;
            }
        }
    }
    
    // Gauss-Jordan elimination.
    for(k=0; k<s2; k++)
    {
        // First first non null element in current column.
        for(j=k; j<s2; j++)
        {
            if(a[k+(j*s2)] != 0)
            {
                break;
            }
        }
        // Empty sub-column.
        if(j >= s2)
        {
            _rank = k;
            break;
        }
        // Swap rows if needed.
        if(j != k)
        {
            for(i=k; i<s2; i++)
            {
                uint8_t v = a[i+(k*s2)];
                a[i+(k*s2)] = a[i+(j*s2)]; 
                a[i+(j*s2)] = v;
            }
            // Don't forget to do the same for the inverse.
            for(i=0; i<s2; i++)
            {
                uint8_t v = _inverse[i+(k*s2)];
                _inverse[i+(k*s2)] = _inverse[i+(j*s2)]; 
                _inverse[i+(j*s2)] = v;
            }
        }
        // Perform elimination
        for(j=0; j<k; j++)
        {
            for(i=0; i<s2; i++)
            {
                _inverse[i+(j*s2)] ^= _inverse[i+(k*s2)] & a[k+(j*s2)];
            }
            for(i=k+1; i<s2; i++)
            {
                a[i+(j*s2)] ^= a[i+(k*s2)] & a[k+(j*s2)];
            }
            a[k+(j*s2)] = 0;
        }
        for(j=k+1; j<s2; j++)
        {
            for(i=0; i<s2; i++)
            {
                _inverse[i+(j*s2)] ^= _inverse[i+(k*s2)] & a[k+(j*s2)];
            }
            for(i=k+1; i<s2; i++)
            {
                a[i+(j*s2)] ^= a[i+(k*s2)] & a[k+(j*s2)];
            }
            a[k+(j*s2)] = 0;
        }
    }
    
    // Save the last 2 columns of a.
    for(j=0; j<s2; j++)
    {
        _basis[0][j] = a[s2-2 + (j*s2)];
        _basis[1][j] = a[s2-1 + (j*s2)];
    }

    delete [] a;
}

} // Merlin
