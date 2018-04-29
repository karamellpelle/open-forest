//    open-demo: an orientering game.
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
#ifndef BATB_DEMO_ITERATION_ITERATIONDEMOFOREST_HPP
#define BATB_DEMO_ITERATION_ITERATIONDEMOFOREST_HPP
#include "BATB/Demo/Iteration/IterationDemo.hpp"
#include "BATB/Demo/Output.hpp"
#include "BATB/Demo/CourseCurve.hpp"
#include "BATB/Forest/Output.hpp"
#include "BATB/Forest/ModifyCamera.hpp"
#include "BATB/Forest/ModifyRunner.hpp"
#include "BATB/Forest/ModifyControlCamera.hpp"
#include "BATB/Forest/ModifyControlRunner.hpp"
#include "BATB/Forest/StepDT.hpp"


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
    IterationDemoForest(BATB* );

    // iterate
    IterationStack iterate_demo(World& world) override;

    // setup before iteration
    void iterate_begin(World& );

    ////////////////////////////////////////

private:
    void createRandomCourse(demo::World& );
    void modifyRunnerDemo(demo::World& );

    demo::Output outputDemo;

    forest::Output output;
    forest::ModifyCamera modifyCamera;
    forest::ModifyRunner modifyRunner;
    forest::ModifyControlCamera modifyControlCamera;
    forest::ModifyControlRunner modifyControlRunner;
    forest::StepDT stepDT;

    // movement of demo runner
    CourseCurve curve;
    uint curve_i = 0;

};



} // namespace demo

} // namespace batb

#endif
