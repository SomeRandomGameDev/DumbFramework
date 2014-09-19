#include "board.hpp"

#include <cstring>

namespace Merlin {

/**
 * Default constructor.
 */
Board::Board()
    : _data(NULL)
    , _size(0)
{}
/**
 * Destructor.
 */
Board::~Board()
{
    destroy();
}

/**
 * Create n*n board.
 * @param [in] n  Board dimension.
 * @param [in] in Board data.
 * @return true upon success.
 */
bool Board::create(size_t n, uint8_t const* in)
{
    // Sanity check.
    if(0 == n)
    {
        return false;
    }
    
    if(n != _size)
    {
        destroy();
        
        _data = new uint8_t[n*n];
        if(NULL == _data)
        {
            return false;
        }
        _size = n;
    }
    
    if(NULL != in)
    {
        memcpy(_data, in, _size*_size*sizeof(uint8_t));
    }
    else
    {
        memset(_data, 0, _size*_size*sizeof(uint8_t));
    }
    return true;
}
/**
 * Destroy board.
 */
void Board::destroy()
{
    if(NULL != _data)
    {
        delete [] _data;
        _data = NULL;
    }
    _size = 0;
}
/**
 * Get board size.
 */
size_t Board::size() const
{
    return _size;
}
/**
 * Get board value at a given coordinate.
 * @param [in] pos Cell coordinate.
 * @return Board value at pos or 0xff if the position was out
 *         of bounds.
 */
uint8_t Board::get(glm::ivec2 const& pos) const
{
    if((static_cast<size_t>(pos.x) >= _size) || (static_cast<size_t>(pos.y) >= _size))
    {
        return 0xff;
    }
    return _data[pos.x + (_size * pos.y)];
}
/**
 * "Push" a board cell.
 * When a cell is pushed, its vertical and horizontal neighbours
 * are switch on or off depending of their original states.
 * @param [in] pos Cell position.
 * @return true if all the cells are off.
 */
bool Board::press(glm::ivec2 const& pos)
{
    int n = static_cast<int>(_size);
    // Sanity check.
    if((pos.x >= n) || (pos.y >= n))
    {
        return false;
    }
    size_t offset = pos.x + (n * pos.y);
    
    // Update cell and its neighbours.
    _data[offset] ^= 1;
    if(pos.x > 0) _data[offset - 1] ^= 1;
    if(pos.y > 0) _data[offset - n] ^= 1;
    if(pos.x < (n-1)) _data[offset + 1] ^= 1;
    if(pos.y < (n-1)) _data[offset + n] ^= 1;
    
    // Check if the board is completely off.
    size_t len = _size*_size;
    for(offset=0; offset<len; offset++)
    {
        if(_data[offset])
        {
            return false;
        }
    }
    
    return true;
}
/**
 * Return the number of lit cells.
 */
unsigned int Board::litCount() const
{
    unsigned int count = 0;
    for(size_t i=0; i<_size; i++)
    {
        count += _data[i];
    }
    return count;
}

} // Merlin
