//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
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


// IterationBegin:
//    encapsulating an existing iteration X, so that the world is modified
//    before the actual iteration X, at the first iteration.
//    the encapsulated Iteration needs a public member 'iterate_begin'
template <typename Iter>
class IterationBegin : public Iteration<typename Iter::World>
{

template <typename Iter_> 
friend Iteration<typename Iter_::World>* begin_iteration(Iter_& );
template <typename Iter_> 
friend Iteration<typename Iter_::World>* begin_iteration(Iter_* );
template <typename Iter_> 
friend Iteration<typename Iter_::World>* begin_iteration(const std::unique_ptr<Iter_>& );

public:

    // modify world, and maybe also the encapsulated 
    // iteration too, before iteration
    IterationStack<typename Iter::World> iterate(typename Iter::World& a)
    {
        // start to iterate
        next_.iterate_begin( a );

        // iterate
        return next_.iterate( a );
    }

private:
    using IterationBase = Iteration<typename Iter::World>;

    IterationBegin(Iter& next) : IterationBase ( std::default_delete<IterationBase>() ),
                                 next_( next ) { }

    // the encapsulated iteration
    Iter& next_;
};


////////////////////////////////////////////////////////////////////////////////
//

// if Iter has a 'void Iter::iterate_begin(Iter::World& )' method, use this:
template <typename Iter>
Iteration<typename Iter::World>* begin_iteration(Iter& iter)
{
    return new IterationBegin<Iter>( iter );
}
template <typename Iter>
Iteration<typename Iter::World>* begin_iteration(Iter* iter)
{
    return new IterationBegin<Iter>( *iter );
}
template <typename Iter>
Iteration<typename Iter::World>* begin_iteration(const std::unique_ptr<Iter>& iter)
{
    return new IterationBegin<Iter>( *iter );
}

// this is _very_ ugly:
// http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
/*
// otherwise, just return adress of argument
template <typename Iter>
Iteration<typename Iter::World>* begin_iteration(Iter& iter)
{
    return &iter;
}
*/

}

#endif
