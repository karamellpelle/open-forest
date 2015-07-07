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
#ifndef GAME_ITERATION_FUNCTION_HPP
#define GAME_ITERATION_FUNCTION_HPP
#include "game/Iteration.hpp"

namespace game
{


// create 'Iteration' from function
template <typename A>
class IterationFunction : public Iteration<A>
{
template <typename A_>
friend IterationFunction<A_>* iteration_function(const typename IterationFunction<A_>::Function& f);

public:
    using Function = std::function<IterationStack<A>(Iteration<A>* , A& )>;
    
    IterationStack<A> iterate(A& a) override { return function_( this, a ); }


private:
    using IterationBase = Iteration<A>;

    IterationFunction(Function f) : IterationBase ( std::default_delete<IterationBase>() ),
                                    function_( f ) { }
    Function function_;

};



////////////////////////////////////////////////////////////////////////////////

template <typename A>
IterationFunction<A>* iteration_function(const typename IterationFunction<A>::Function& f)
//IterationFunction<A>* iteration_function(typename IterationFunction<A>::Function f)
{
    return new IterationFunction<A>( f );
}


}

#endif
