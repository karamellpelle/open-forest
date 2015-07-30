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
#ifndef BATB_FOREST_RUNNER_HPP
#define BATB_FOREST_RUNNER_HPP
#include "batb/batb_include.hpp"
#include "batb/run/Player.hpp"


namespace batb
{

class BATB;


namespace forest
{


class Runner;


/*
class Runner::Trace
{
public:
    class TracePoint
    {
    public:
        TracePoint(tick_t t, const glm::vec4& p) : tick( t ), pos( p ) { } 
        tick_t tick = 0.0;
        glm::vec4 pos; // TODO: mat4
    };

    void push(const TracePoint& p)
    {
        points.push_back( point );
        point = p;
    }

    TracePoint point;
    std::vector<TracePoint> points;

private:
};

*/


class Runner
{
public:
  
    // Player defines who is running. Player is a part of RunWorld.
    // Player defines stuff like network computer, name, ...
    run::Player* player = nullptr; 

    // Map, from ForestWorld, this Runner is using. 
    Map* map = nullptr;

    float_t speed = 0.0;

    glm::vec4 pos;
    glm::mat4 aim;
    // ^ TODO: merge into 1 mat4

    //ogre_
    //al_

    //  if ( length pos -> pos_next > epsilon ) trackPush( dt );
    //    void trackPush(tick_t dt); // push current physics to track at point 'dt'
    //list (tick_t, pos, aim ) track;
    //Trace trace;

    // a Runner does not imply a Course. Runner's are free to do whatever
    // they will, in a forest.
    // a Course for a Runner may come from competition/training/etc.
    // specific events like competition/training are driven by specific
    // Iteration-classes. so it is up to these classes to assign a Course
    // to a Player. 
    // TODO:  think through this. for example the possibility to have independent
    //        training and competition for different Runner's at the same time. 

    // physical stuff
    // * position (relative to Terrain)
    // * speed    ( ... )
    // ...

    // Ground* ground_;       // to be updated by movement. pointer/value
    // Ground* ground_prev_;  // to something describing where standing

    // gameplay stuff
    // * fatigue
    // ...

    // output stuff
    // * animation
    // ...
};

} // namespace forest

} // namespace batb



#endif
