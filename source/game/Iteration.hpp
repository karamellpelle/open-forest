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
#ifndef GAME_ITERATION_HPP
#define GAME_ITERATION_HPP
#include "game/game_include.hpp"



namespace game
{


// IterationStack owns Iterations, hence have the possibility to
// release memory when no more references to an Iteration
template <typename A>
class IterationStack;





// the Iteration base class
template <typename A>
class Iteration 
{
friend class IterationStack<A>;

public:
    // our world type
    typedef A World;

    // our delete type
    //using Deleter = [](Iteration<A>*)->void;
    typedef void (*Deleter)(Iteration<A>*);

    virtual ~Iteration() { }

    // define this in subclass:
    virtual void iterate(IterationStack<A>& stack, A& a) = 0;


protected:
    Iteration() : Iteration( nullptr )    { }
    Iteration(Deleter d) : deleter_( d )  { }


private:
    // autorelease
    // FIXME: implement copy/assigment/move
    uint count_         = 0;              
    Deleter deleter_    = nullptr;

};


////////////////////////////////////////////////////////////////////////////////
//  deleters
//

template <typename A>
void delete_new(Iteration<A>* iter)
{
    delete iter;
}


} // namespace game

#endif


