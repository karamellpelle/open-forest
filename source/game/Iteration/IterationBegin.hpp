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



// encapsulating an existing iteration X, so that the world is modified
// before the actual iteration X. 
// implement world_begin.
template <typename A, typename Iter>
class IterationBegin : public Iteration<A>
{

template <Iter, A> friend
IterationBegin<Iter, A> begin_iteration(Iter* );

public:

    void iterate(IterationStack<A>& stack, A& a)
    {
        // start to iterate
        next_->iterateBegin( a );

        // iterate
        next_->iterate( stack, a );
    }

private:
    IterationBegin(Iter* next) : next_(next) { }

    // 
    Iter* next_;
};



// FIXME: implement a specialized version of this for Iteration's
//        not having a 'iterateBegin' member
template <typename A, typename Iter>
IterationBegin<A, Iter> begin_iteration(Iter* iter)
{
    IterationBegin<A, Iter> ret = new IterationBegin<A, Iter>( iter );

    // IterationBegin are auto released
    Ref::hold( ret );

    return ret;
}



}

#endif
