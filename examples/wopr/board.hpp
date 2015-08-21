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
#ifndef _BOARD_H_
#define _BOARD_H_

#include <algorithm>
#include <functional>

namespace WOPR {

/// Game status.
enum Status
{
    OutOfBound = -2,
    AlreadySet = -1,
    OK = 0,
    Win,
    Loose,
    Draw
};

/// This a classical tic-tac-toe board.
/// It is a 9 per 9 board. Each element is either a circle,
/// a cross or empty. 
class Board 
{
    public:
        /// Board item.
        enum Item
        {
            Circle = -1,
            Empty  =  0,
            Cross
        };
        
    public:
        /// Default constructor.
        Board();
        /// Destructor.
        ~Board();
        
        /// Put an item on a given cell.
        /// @param [in] x    Cell X coordinates.
        /// @param [in] y    Cell y coordinates.
        /// @param [in] item Item to put in the cell.
        /// @return
        ///     @b OK         if the item was successfully put.
        ///     @b Win        if the current item won.
        ///     @b Draw       if the board is full and nobody wins.
        ///     @b AlreadySet if the cell was not empty.
        ///     @b OutOfBound if the position is not in the board range.
        Status put(int x, int y, Item item);

        /// Undo last move.
        /// @return @b false if there is no move to undo, @b true otherwise.
        bool undo();
        
        /// Check board state for a given item.
        /// @return
        ///     @b Draw   If the board is full and nobody won.
        ///     @b Win    If the current item won.
        ///     @b Loose  If the current item lost.
        ///     @b Ok     If the game is not finished.
        Status check(Item item);
        
        /// Tells if the board empty.
        /// @return @b true if the board is empy, @b false otherwise.
        bool empty();
        
        /// Clear board.
        /// Set all cells to @b Empty.
        void clear();
        
        /// Run functor on each cell of the board.
        /// @param [in] visitor Board visitor.
        void foreach(std::function<void(int, int, Item)> visitor);
        
        /// Finds the best position for a given item.
        /// @param [in]  current Current item.
        /// @param [out] x       Best cell X position.
        /// @param [out] y       Best cell y position.
        void find_best(Item current, int &x, int &y);
        
    private:
        /// The infamous minimax.
        /// @param [in] root    This is the item for which we are trying to find the best position.
        /// @param [in] current Current item.
        /// @return Best rank.
        int minimax(Item root, Item current);

    private:
        /// Board cells.
        Item _cell[9];
        /// Current status.
        /// Win or Loose are wrt Cross item.
        Status _status;
        /// History.
        struct
        {
            /// Cell X position where the item was put.
            int x;
            /// Cell Y position where the item was put.
            int y;
            /// Board status.
            Status status;
        } _history[9];
        /// Current history index.
        int _time;
};

} // WOPR

#endif // _BOARD_H_
