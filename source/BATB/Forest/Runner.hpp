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
#ifndef BATB_FOREST_RUNNER_HPP
#define BATB_FOREST_RUNNER_HPP
#include "BATB/Forest/DTMovable.hpp"

namespace Ogre
{
class Entity;
class AnimationState;
class Light;
}


namespace batb
{

class Player;

  
namespace forest
{

class World;
class Control;
class Map;
class Runner;


// point in a trace
class TracePoint
{
public:
    TracePoint() { }
    TracePoint(tick_t t, const glm::vec3& v) : tick( t ), x( v[0] ), y( v[1] ), z( v[2] )  { }
      
    tick_t tick = 0.0;
  
    // use 'float' to save mem?
    float_t x;
    float_t y;
    float_t z;

};


// line of TracePoint's
class Trace
{
public:
    void push(const TracePoint& point1);
    void pushIf(float_t epseps, const TracePoint& point1)
    {
        float_t x = point1.x - point0.x;
        float_t y = point1.y - point0.y;
        float_t z = point1.z - point0.z;
        if ( epseps <= x * x + y * y + z * z )
        {
            push( point1 );
        }
    }
    void position(const TracePoint& p)
    {
        point0 = p;
    }
    void reset(const TracePoint& p)
    {
        points.clear();
        point0 = p;
    }

    using TracePoints = std::vector<TracePoint>;
    uint size() const { return points.size(); }

    TracePoints points;
    TracePoint point0;

private:
};

////////////////////////////////////////////////////////////////////////////////
// RunnerAnimation

// tmp
class RunnerAnimation
{
public:
    RunnerAnimation(Runner& r) : runner( r ) { }

    void reset(const YAML::Node& );
    void step(tick_t );

    Ogre::AnimationState* ogre_animstate_base = nullptr;
    Ogre::AnimationState* ogre_animstate_top = nullptr;

    tick_t tick = 0.0;
    Runner& runner;
};

  

/////////////////////////////////////////////////////////////////////////////// 
// Runner
//


class Runner
{
public:

    Runner(World& w, Player* p = nullptr);

    // assignment/copy allowed, if object copy, create 'clone()'
    Runner(const Runner& );
    Runner(Runner&& );

    ////////////////////////////////////////////////////////////////////////////////
    void reset(const YAML::Node& );
    void reset(const glm::vec2& );
    void reset(const glm::vec3& );

    void headlampOn(bool );

    // update position and output-state
    void step();

    // punch a control
    void punch(Control* );

    // inside world
    World& forest;

    // Player defines who is running. Player is a part of RunWorld.
    // Player defines stuff like name, network computer, ...
    Player* player = nullptr; 

    // Map, from ForestWorld, this Runner is using. 
    Map* map = nullptr;


    // physical state
    DTMovable move;

    // previous control
    Control* control0 = nullptr;

    //  if ( length pos -> pos_next > epsilon ) trackPush( dt );
    //    void trackPush(tick_t dt); // push current physics to track at point 'dt'
    Trace trace;

    // a Runner does not imply a Course. Runner's are free to do whatever
    // they will in a forest.
    // a Course for a Runner may come from competition/training/etc.
    // specific events like competition/training are driven by specific
    // Iteration-classes. so it is up to these classes to assign a Course
    // to a Player. For example race::World containing forest::World
    // TODO:  think through this. for example the possibility to have independent
    //        training and competition for different Runner's at the same time. 


    // gameplay stuff
    // * fatigue
    // ...
    float_t intensity = 0.0;

    ////////////////////////////////////////////////////////////////////////////////
    // output 


    //ogre_
    // our runners SceneNode is the one of 'ogre_entity'
    Ogre::Entity* ogre_entity         = nullptr;
    Ogre::Light* ogre_headlamp        = nullptr; // ->setType(Light::LT_SPOTLIGHT); see https://ogrecave.github.io/ogre/api/1.11/tut__lights_cameras_shadows.html

    // sound
    //AL::source al_source

    RunnerAnimation animation;

};

} // namespace forest

} // namespace batb



#endif
