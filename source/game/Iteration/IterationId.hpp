//    orienteering-game: an orientering game.
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
#ifndef GAME_ITERATION_ITERATION_ID_HPP
#define GAME_ITERATION_ITERATION_ID_HPP
#include "game/Iteration.hpp"


namespace game
{


// "identity iteration"
template <typename A>
class IterationId : public Iteration<A>
{
template <typename B>   // shadows A???
friend IterationId<B>* iteration_id();

public:
    using Iteration<A>::Iteration;

    //IterationId() : Iteration<A>() { }

    // empty work
    void iterate(IterationStack<A>& stack, A& a) { } 

};



////////////////////////////////////////////////////////////////////////////////
//
//

// deleter for Iteration's created by 'new'
template <typename A>
IterationId<A>* iteration_id()
{
    return new IterationId<A>( delete_new<A> );
}

}

#endif
