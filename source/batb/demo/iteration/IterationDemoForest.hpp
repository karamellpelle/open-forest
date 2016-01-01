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
#ifndef BATB_DEMO_ITERATION_ITERATIONDEMOFOREST_HPP
#define BATB_DEMO_ITERATION_ITERATIONDEMOFOREST_HPP
#include "batb/demo/iteration/IterationDemo.hpp"
#include "batb/demo/BeginEvents.hpp"
#include "batb/demo/Output.hpp"
#include "batb/demo/CourseCurve.hpp"
#include "batb/forest/Output.hpp"
#include "batb/forest/BeginEvents.hpp"
#include "batb/forest/ModifyCamera.hpp"
#include "batb/forest/ModifyRunner.hpp"
#include "batb/forest/ModifyControlCamera.hpp"
#include "batb/forest/ModifyControlRunner.hpp"
#include "batb/forest/StepDT.hpp"


namespace batb
{


class BATB;

namespace demo
{




// IterationDemoForest:
//
class IterationDemoForest : public IterationDemo
{
public:
    // construct from the containing BATB
    IterationDemoForest(BATB& );

    // iterate
    IterationStack iterate_demo(World& world) override;

    // setup before iteration
    void iterate_begin(World& );

    ////////////////////////////////////////

private:
    void createCourse(demo::World& );
    void modifyRunnerDemo(demo::World& );

    demo::Output outputDemo;
    demo::BeginEvents beginEventsDemo;

    forest::Output output;
    forest::BeginEvents beginEvents;
    forest::ModifyCamera modifyCamera;
    forest::ModifyRunner modifyRunner;
    forest::ModifyControlCamera modifyControlCamera;
    forest::ModifyControlRunner modifyControlRunner;
    forest::StepDT stepDT;

    // movement of demo runner
    CourseCurve curve;
    uint curve_i = 0;

};

////////////////////////////////////////////////////////////////////////////////
//  

void begin(IterationDemoForest& );


void end(IterationDemoForest& );



} // namespace demo

} // namespace batb

#endif
