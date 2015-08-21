/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "board.hpp"

#include <cstring>
 
namespace WOPR {
/// Default constructor.
Board::Board()
{
    clear();
}
/// Destructor.
Board::~Board()
{}
/// Put an item on a given cell.
/// @param [in] x    Cell X coordinate.
/// @param [in] y    Cell y coordinate.
/// @param [in] item Item to put in the cell.
/// @return
///     @b OK         if the item was successfully put.
///     @b Win        if the current item won.
///     @b Draw       if the board is full and nobody wins.
///     @b AlreadySet if the cell was not empty.
///     @b OutOfBound if the position is not in the board range.
Status Board::put(int x, int y, Board::Item item)
{
    if((x < 0) || (x >= 3) || (y < 0) || (y >= 3))
    {
        return OutOfBound;
    }
    int index = x + (3*y);
    if(Board::Empty != _cell[index])
    {
        return AlreadySet;
    }

    _history[_time].x = x;
    _history[_time].y = y;
    _history[_time].status = _status;
    _time++;

    _cell[index] = item;
    
    int count[4] = {0, 0, 0, 0};
    for(int d=0; d<3; d++)
    {
        // Check horizontally.
        if(item == _cell[d+(3*y)])
        {
            ++count[0];
        }
        // Check vertically.
        if(item == _cell[x+(3*d)])
        {
            ++count[1];
        }
        // Check first diagonal.
        if(item == _cell[d+(3*d)])
        {
            ++count[2];
        }
        // Check second diagonal.
        if(item == _cell[(2-d)+(3*d)])
        {
            ++count[3];
        }
    }

    if((3 == count[0]) || (3 == count[1]) || (3 == count[2]) || (3 == count[3]))
    {
        _status = (Board::Cross == item) ? Win : Loose;
        return Win;
    }
    if(9 == _time)
    {
        _status = Draw;
    }
    return _status;
}
/// Undo last move.
/// @return @b false if there is no move to undo, @b true otherwise.
bool Board::undo()
{
    if(0 == _time)
    {
        return false;
    }
    --_time;
    int index = _history[_time].x + (3*_history[_time].y);
    _cell[index] = Board::Empty;
    _status = _history[_time].status;
    return true;
}
/// Tells if the board empty.
/// @return @b true if the board is empy, @b false otherwise.        
bool Board::empty()
{
    return (0 == _time);
}
/// Check board state for a given item.
/// @return
///     @b Draw   If the board is full and nobody won.
///     @b Win    If the current item won.
///     @b Loose  If the current item lost.
///     @b Ok     If the game is not finished.
Status Board::check(Board::Item item)
{
    if(Board::Cross != item)
    {
        if(Win == _status)
        {
            return Loose;
        }
        else if(Loose == _status)
        {
            return Win;
        }
    }
    return _status;
}
/// Clear board.
/// Set all cells to @b Empty.
void Board::clear()
{
    _status = OK;
    memset(_cell, Board::Empty, sizeof(_cell));
    
    _time = 0;
    memset(_history, 0, sizeof(_history));
}
/// Run functor on each cell of the board.
/// @param [in] visitor Board visitor.
void Board::foreach(std::function<void(int, int, Item)> visitor)
{
    for(int y=0; y<3; y++)
    {
        for(int x=0; x<3; x++)
        {
            visitor(x, y, _cell[x+(3*y)]);
        }
    }
}
/// Finds the best position for a given item.
/// @param [in]  current Current item.
/// @param [out] x       Best cell X position.
/// @param [out] y       Best cell y position.
void Board::find_best(Board::Item current, int &x, int &y)
{
    // If the board is empty, generate a random position.
    if(empty())
    {
        std::random_device rd;
        std::uniform_int_distribution<int> dis(0,2);
        x = dis(rd);
        y = dis(rd);
        return;
    }
    // Find the best position using minmax.
    int best = -1;
    Board::Item opposite = static_cast<Board::Item>(-current);
    for(int j=0; j<3; j++)
    {
        for(int i=0; i<3; i++)
        {
            if(Board::Empty == _cell[i+(3*j)])
            {
                int rank;
                put(i, j, current);
                rank = minimax(current, opposite);
                undo();
                if(rank >= best)
                {
                    best = rank;
                    x = i;
                    y = j;
                }
            }
        }
    }
}
/// The infamous minimax.
/// @param [in] root    This is the item for which we are trying to find the best position.
/// @param [in] current Current item.
/// @return Best rank.
int Board::minimax(Board::Item root, Board::Item current)
{
    Status status = check(root);
    if(OK != status)
    {
        if(Draw == status)
        {
            return 0;
        }
        if(Win == status)
        {
            return 1;
        }
        if(Loose == status)
        {
            return -1;
        }
    }
    
    int multiplier = current * root;
    
    int best = -1;
    Board::Item opposite = static_cast<Board::Item>(-current);
    for(int j=0; j<3; j++)
    {
        for(int i=0; i<3; i++)
        {
            if(Board::Empty == _cell[i+(3*j)])
            {
                int rank;
                put(i, j, current);
                rank = multiplier * minimax(root, opposite);
                undo();
                if(rank >= best)
                {
                    best = rank;
                }
            }
        }
    }
    return multiplier*best;
}

} // WOPR
