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
#ifndef BATB_RUN_ITERATION_ITERATIONRUNDEMO_HPP
#define BATB_RUN_ITERATION_ITERATIONRUNDEMO_HPP
#include "BATB/Run/Iteration/IterationRun.hpp"
#include "BATB/Demo/Iteration/IterationDemo.hpp"
#include "BATB/Demo/workers.hpp"
#include "BATB/Demo/World.hpp"


namespace batb
{


class BATB;

namespace run
{

// IterationRunDemo:
//    loads the non-core part of BATB (and afterwards run::World??), 
//    showing the load progress interactively. finishes when load error
//    or load complete. continues with IterationRunMain if success loading...
class IterationRunDemo : public IterationRun
{
public:
    // construct from the containing Run object
    IterationRunDemo(BATB* );

    // setup 
    void begin();

    // set demo world this works on
    void demoWorld(demo::World* demo);

    // iterate
    IterationStack iterate_run(World& world) override;

    // setup before iteration
    void iterate_begin(World& );

    ////////////////////////////////////////

private:
    demo::World* demo_ = nullptr;
    demo::IterationStack stack_;

};

////////////////////////////////////////////////////////////////////////////////
//  



} // namespace run

} // namespace batb

#endif
