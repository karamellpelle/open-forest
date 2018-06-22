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
#ifndef BATB_DEMO_WORLD_HPP
#define BATB_DEMO_WORLD_HPP
#include "BATB/Demo/Course.hpp"
#include "BATB/Demo/DemoMapDrawer.hpp"
#include "BATB/Forest/WorldDrawer.hpp"


namespace batb
{

namespace run    { class World; }
namespace forest { class World; }
namespace forest { class Runner; }

namespace demo
{




class World
{

public:
    World(run::World* r);


    // subworld: forest::World
    std::unique_ptr<forest::World> forest;

    // drawing state: 3D world
    forest::WorldDrawer forest_drawer;
    // drawing state: 2D map
    demo::DemoMapDrawer map_drawer;
    // physics update state
    //forest::StepDT stepdt;

    // controllers
    //forest::AimKeysController     aim_controller;
    //forest::MapViewKeysController map_controller;

    // working with Runner
    forest::Runner* runner = nullptr;
    //forest::AimRunnerController runnercontroller_aim;
    //demo::AIRunnerController    runnercontroller_demoai;

    // demo values
    Course course;
    uint course_i = 0; // current control



    ////////////////////////////////////////////////////////////////

    // tick of world
    tick_t tick = 0.0;

    EventList events;


    // TODO: remove?
    run::World* run = nullptr;


};


} // namespace demo

} // namespace batb




#endif
