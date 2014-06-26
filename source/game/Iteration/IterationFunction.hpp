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
#ifndef ITERATION_FUNCTION_HPP
#define ITERATION_FUNCTION_HPP
#include "../Iteration.hpp"

namespace Game
{


// create 'Iteration' from function
template <typename A>
class IterationFunction : public Iteration<A>
{
public:
    typedef void (*FunctionT)(Iteration<A>* , IterationStack<A>& , A& );


    // create/destroy
    static IterationFunction<A>* create(FunctionT f) { return new IterationFunction( f ); }   // create
    static void destroy(IterationFunction<A>* iter)  { delete iter; }                        // free mem, for user-release

    void iterate(IterationStack<A>& stack, A& a) { function_(this, stack, a); }

protected:
    IterationFunction(FunctionT f) : function_( f ) { }
    ~IterationFunction() { } 
    void destroy()
    {
        delete this;
    }

private:

    FunctionT function_;

};


}

#endif
