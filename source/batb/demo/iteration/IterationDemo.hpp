//    open-demo: an orientering game.
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
#ifndef BATB_DEMO_ITERATION_ITERATION_DEMO_HPP
#define BATB_DEMO_ITERATION_ITERATION_DEMO_HPP
#include "batb/batb_include.hpp"
#include "batb/demo/World.hpp"

namespace batb
{


class BATB;



namespace demo
{

// the type of stack for demo::Iteration
using IterationStack = game::IterationStack<World>;


// the type of iterations for demo::World
class IterationDemo : public game::Iteration<World>
{
public:
    IterationDemo(BATB& b);

    // this handles each frame, delegating work to the subclass 
    // implementation of 'iterate_demo'
    virtual IterationStack iterate(World& ) final;

    // also, each subclass should typically create a non-virtual method:
    // void iterate_begin(World& );

    BATB& batb;

protected:
    // subclasses implements this:
    virtual IterationStack iterate_demo(World& ) = 0;


};

}

}

#endif
