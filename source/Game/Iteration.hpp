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
#ifndef ITERATION_HPP
#define ITERATION_HPP
#include "GameInclude.hpp"
#include "Ref.hpp"



namespace Game
{

template <typename A>
class IterationStack;


//template <Env=void, A>
template <typename A>
class Iteration : public Ref
{
friend class IterationStack<A>;
public:
    //Iteration(Env* env) : env_( env ) { }
    virtual ~Iteration() { }                  // FIXME: proctected, for named destructors?


    // I prefer named constructors and destructors for Iteration, 
    // so for subclass IterationX, I create static functions:
    //
    // static IterationX<A>*  create(Y y)             { ... }
    // static void            destroy(Iteration<A>* ) { ... }
    // 
    // , and let the constructor be private


    // define this in subclass:
    virtual void iterate(IterationStack<A>& stack, A& a) = 0;

    //Env* env() { return env_; }
private:
    //Env* env_;

};

}

#endif


