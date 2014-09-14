#include "board.hpp"

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
 * @param [in] n Board dimension.
 * @return true upon success.
 */
bool Board::create(size_t n)
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
    
    memset(_data, 0, _size*_size*sizeof(uint8_t));
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
    if((pos.x >= _size) || (pos.y >= _size))
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
    // Sanity check.
    if((pos.x >= _size) || (pos.y >= _size))
    {
        return false;
    }
    size_t offset = pos.x + (_size * pos.y);
    
    // Update cell and its neighbours.
    board[offset] ^= 1;
    if(pos.x > 0) _data[offset - 1] ^= 1;
    if(pos.y > 0) _data[offset - _size] ^= 1;
    if(pos.x < (_size-1)) _data[offset + 1] ^= 1;
    if(pos.y < (_size-1)) _data[offset + _size] ^= 1;
    
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

} // Merlin
