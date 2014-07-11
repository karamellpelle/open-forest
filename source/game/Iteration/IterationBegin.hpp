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
#ifndef GAME_ITERATION_BEGIN_HPP
#define GAME_ITERATION_BEGIN_HPP
#include "game/Iteration.hpp"



namespace game
{


// IterationBegin
//    encapsulating an existing iteration X, so that the world is modified
//    before the actual iteration X, at the first iteration.
//    the encapsulated Iteration needs a public member 'iterate_begin'
//    TODO: create special template for 'begin_iteration' for Iteration's 
//          without 'iteratie_begin', doing nothing.
//template <typename A, template <typename> class Iter >
template <typename A, typename Iter>
class IterationBegin : public Iteration<A>
{

template <typename Iter_> 
friend IterationBegin<typename Iter_::World, Iter_>* begin_iteration(Iter_& );

public:

    // modify world, and maybe also the encapsulated 
    // iteration too, before iteration
    void iterate(IterationStack<A>& stack, A& a)
    {
        // start to iterate
        next_.iterate_begin( a );

        // iterate
        next_.iterate( stack, a );
    }

private:
    IterationBegin(Iter& next) : Iteration<A>( delete_new ), next_(next) { }

    // the encapsulated iteration
    Iter& next_;
};


////////////////////////////////////////////////////////////////////////////////
//

// FIXME: genarized, and specialized version below...
template <typename Iter>
IterationBegin<typename Iter::World, Iter>* begin_iteration(Iter& iter)
{
    return new IterationBegin<typename Iter::World, Iter>( iter );
}


}

#endif
