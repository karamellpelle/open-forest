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
#ifndef GAME_ITERATION_HPP
#define GAME_ITERATION_HPP
#include "game/game_include.hpp"
#include "game/IterationStack.hpp"



namespace game
{







// the Iteration base class
template <typename A>
class Iteration 
{
template <typename A_>
friend void iteration_release(Iteration<A_>& iteration);
template <typename A_>
friend void iteration_hold(Iteration<A_>& iteration);


public:

    // our world type
    //typedef A World;
    using World = A;

    // our delete type
    //using Deleter = [](Iteration<A>*)->void;
    //typedef std::function<void(Iteration<A>*)> Deleter;
    using Deleter = std::function<void(Iteration<A>*)>;
    

    virtual ~Iteration() { }

    // define this in subclass:
    virtual IterationStack<A> iterate(A& a) = 0;


protected:
    //Iteration() = default;
    Iteration() : deleter_( [](Iteration<A>* i) -> void { } ) { }
    Iteration(const Deleter& d) : deleter_( d )  { }
    //Deleter deleter_ = [](Iteration<A>* i) -> void { }; <- internal compiler error :(


private:
    // autorelease
    // TODO: implement copy/assigment/move
    uint count_         = 1;              
    Deleter deleter_;

};



////////////////////////////////////////////////////////////////////////////////
template <typename A>
void iteration_release(Iteration<A>& iteration)
{
    uint& count = iteration.count_;
    if ( count != 0 )
    {
        if ( --count == 0 )
        {
            // call deleter, since no more references 
            iteration.deleter_( &iteration );
        }
    }
}


template <typename A>
void iteration_hold(Iteration<A>& iteration)
{
    uint& count = iteration.count_;
    ++count; 
}


} // namespace game

#endif


