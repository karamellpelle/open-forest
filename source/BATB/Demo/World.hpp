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
    World(run::World& r);

    // setup this world. this should ideally be done by IterationRunDemoBegin,
    // and not call this function, since loading takes time and should be done
    // in seperate thread
    void begin();

    ////////////////////////////////////////////////////////////////
    run::World& run;

    // forest subworld
    std::unique_ptr<forest::World> forest;

    EventList events;

    // tick of world
    tick_t tick = 0.0;

    
    forest::Runner* runner = nullptr;

    Course course;
    uint course_i = 0; // current control


private:
    
};


} // namespace demo

} // namespace batb




#endif
