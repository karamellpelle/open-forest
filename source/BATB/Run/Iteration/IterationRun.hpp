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
#ifndef BATB_RUN_ITERATION_ITERATIONRUN_HPP
#define BATB_RUN_ITERATION_ITERATIONRUN_HPP
#include "game.hpp"
#include "BATB/Run/World.hpp"

namespace batb
{


class BATB;
class Scene;



namespace run
{

// the type of stack for run::Iteration
using IterationStack = game::IterationStack<World>;


// the type of iterations for run::World
class IterationRun : public game::Iteration<World>
{
public:
    IterationRun(BATB* b);

    // this handles each frame, delegating work to the subclass 
    // implementation of 'iterate_run'
    virtual IterationStack iterate(World& ) final;

    BATB* batb = nullptr;

protected:
    // subclasses implements this:
    virtual IterationStack iterate_run(World& ) = 0;


private:
    void begin(Scene& );
};

}

}

#endif
