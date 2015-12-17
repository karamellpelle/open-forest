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
#ifndef BATB_DEMO_WORLD_HPP
#define BATB_DEMO_WORLD_HPP
#include "batb/batb_include.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb/event.hpp"


namespace batb
{

namespace run
{
class World;
}

namespace demo
{


//class ControlPoint
//{
//public:
//    ControlPoint(const glm::vec2& v) : point( v ) { }
//
//    forest::Control* control = nullptr;
//    glm::vec2 point;
//};


class World
{

public:
    World(run::World& r) : run( r ), forest( r ) { }

    run::World& run;
    EventList events;
    tick_t tick = 0.0;
/*
    // 
    ControlPoint cpoint;
    std::forward_list<ControlPoint> cpoint_nexts;
    std::forward_list<ControlPoint> cpoint_prevs;


*/
    // forest subworld
    forest::World forest;
    //forest::IterationStack forest_stack;
    
    //forest::Runner* runner;

    // tick of world

private:
    
};


} // namespace demo

} // namespace batb




#endif
