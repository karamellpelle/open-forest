//    open-forest: an orientering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#ifndef GAME_ITERATE_HPP
#define GAME_ITERATE_HPP
#include <forward_list>
#include "game/Iteration.hpp"


namespace game
{





////////////////////////////////////////////////////////////////////////////////


template <typename A>
void clear(IterationStack<A>& stack)
{
    while ( !stack.empty() )
    {
        iteration_release( stack.front() );
        stack.pop_front();
    }
}




template <typename A>
void iterate(IterationStack<A>& stack, A& a)
{
    if ( !stack.empty() )
    {

        // make 1 iteration on 'a'
        auto top = stack.front()->iterate( a );

        // pop 'iteration'
        iteration_release( *stack.front() );
        stack.pop_front();

        // hold 1 reference to each new element for stack (top)
        for (auto& i : top)
        {
            iteration_hold( *i );
        }
        // insert top to stack
        stack.insert_after( stack.before_begin(), std::begin( top ), std::end( top ) );



    }
}

    


} // namespace game

#endif

