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
#ifndef ITERATION_STEP_HPP
#define ITERATION_STEP_HPP
#include "Ref.hpp"
#include "Iteration.hpp"


namespace Game
{


template <typename A>
class Step : public Ref
{
friend class Iteration<A>;
public:
    virtual ~Step() { }       // FIXME: protected?

    // we prefer named constructors, so for subclass IterationX, 
    // make static 'create' functions:
    //  static OutputX* create(Y y)

    virtual void stepWorld(Iteration<A>* iter, IterationStack<A>& stack, A& a) = 0;


protected:
    Step() : iteration_( 0 ) { } 

    // if this belongs to som Iteration
    Iteration<A>* iteration_;  


};


}

#endif
