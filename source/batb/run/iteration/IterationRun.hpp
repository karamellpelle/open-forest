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
#ifndef BATB_RUN_ITERATION_ITERATION_RUN_HPP
#define BATB_RUN_ITERATION_ITERATION_RUN_HPP
#include "batb/batb_include.hpp"
#include "batb/run/World.hpp"

namespace batb
{


class BATB;
class Scene;



namespace run
{

// the type of stack for run::Iteration
using IterationStack = game::IterationStack<World>;


// the type of iterations for run::World
//using Iteration = game::Iteration<World>;
class IterationRun : public game::Iteration<World>
{
public:
    IterationRun(BATB& b);

    // this handles each frame, delegating work to subclass 
    // implementation of 'iterate_run'
    virtual void iterate(IterationStack& , World& ) final;

    // FIXME:
    //virtual void iterate_begin(World& ) final;

    BATB& batb;

protected:
    // subclasses implements this:
    virtual void iterate_run_begin(IterationStack& , World& ) = 0;

    // FIXME:
    //virtual void begin_iterate_run(World& ) = 0;

private:
    void begin(Scene& );
};

}

}

#endif
