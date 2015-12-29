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
#include "batb/forest/Output.hpp"
#include "batb/forest/BeginEvents.hpp"
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

    demo::Output outputDemo;
    demo::BeginEvents beginEventsDemo;

    forest::Output output;
    forest::BeginEvents beginEvents;
    forest::ModifyControlCamera modifyControlCamera;
    forest::ModifyControlRunner modifyControlRunner;
    forest::StepDT stepDT;

    //float_t aim_a = 0.0;  // yaw
    //float_t aim_b = 0.0;  // pitch
    //float_t aim_c = 0.0;  // roll

    //Runner* aiming = nullptr;

    //float_t map_scale_0_ = 0;
    //float_t map_scale_1_ = 0;
    //glm::vec2 map_pos_1_;
    //glm::vec2 map_dir_0_;
 
    //using BezierPoints = std::vector<glm::vec2>;

    //BezierPoints beziers_;
    //BezierPoints random(const glm::vec2& , const glm::vec2& );
    //void tmp_show_bezier(World& );
    //glm::vec2 p0;
    //glm::vec2 p1;
};

////////////////////////////////////////////////////////////////////////////////
//  

void begin(IterationDemoForest& );


void end(IterationDemoForest& );



} // namespace demo

} // namespace batb

#endif
