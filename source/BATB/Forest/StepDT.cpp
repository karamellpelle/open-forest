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
#include <iomanip>
#include "BATB/Forest.hpp"
#include "BATB/Forest/StepDT.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/events.hpp"
#include "BATB/Value/Forest.hpp"


#define KEEP_CAMERA_ABOVE_GROUND


namespace batb
{

namespace forest
{

StepDT::StepDT()
{

}

void stepdt(DTMovable& move, tick_t dt)
{
    // currently, only the position part (mat4x4[ 3 ]) is updated
    
    move.acc[3] = 1.0;

    move.vel += (float)( dt ) * move.acc;
    move.vel[3] = 1.0;

    move.pos += (float)( dt ) * move.vel;
    move.pos[3] = 1.0;

    //move.pos = pos;
    //move.vel[3] = vel;
    //move.acc[3] = acc;
    
    move.computed = false;
}


void StepDT::add(World& forest, tick_t dt)
{
    // TODO: Camera and Runner should have pointers to DTMovable.
    //       all DTMovable's should be in continuos memory. 
    //       hence update of dt can be done directly and fast
    //       by compute all of the DTMovable's in one bulk.

    ////////////////////////////////////////////////////////////////////////////////
    // step camera
    // 
    Camera& camera = forest.camera;
    stepdt( camera.move, dt );

#ifdef KEEP_CAMERA_ABOVE_GROUND
    {
    auto pos = camera.move.pos;
    auto terrain_h = forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( Ogre::Vector3( pos.x, pos.y, pos.z ) );
    camera.move.pos.y = std::max( pos.y, 1 + terrain_h ); 
    }
#endif

    // also set listener to camera Aim
    auto listener = forest.alure_listener;
    auto pos = camera.move.aim[3];
    auto at = camera.move.aim[2]; // FIXME: correct camera Aim matrix
    auto up = camera.move.aim[1];
    forest.alure_listener.setPosition( { pos.x, pos.y, pos.z } );
    forest.alure_listener.setOrientation( {{at.x, at.y, at.z}, {up.x, up.y, up.z}} );
   
    
    ////////////////////////////////////////////////////////////////////////////////
    // step runners
    // 
    for (auto i = std::begin( forest.runners ); i != std::end( forest.runners ); ++i)
    {
        const auto tick_next = forest.tick + dt;

        auto& runner_a = *i;

        // step runner
        stepdt( runner_a.move, dt );
      
        // ensure runner above terrain
        auto pos = runner_a.move.pos;
        runner_a.move.pos.y = 14 + forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( 
                                   Ogre::Vector3( pos.x, pos.y, pos.z ) );

        // collide with controls
        for (auto j = std::begin( forest.controls ); j != std::end( forest.controls ); ++j)
        {
            auto& control = *j;

            // see if runner is close to control
            auto diff = runner_a.move.pos - control.aim.pos;
            float_t epseps = glm::dot( diff, diff );
            if ( epseps < value::forestProximityControl )
            {
                // event::ProximityControl
                forest.events.push( event::ProximityControl( tick_next, &control, &runner_a, epseps ) );
            }
        }

        // collide with other runners
        auto j = i;
        for (++j; j != std::end( forest.runners ); ++j)
        {
            auto& runner_b = *j;

            // see if runner is close to runner
            auto diff = runner_a.move.pos - runner_b.move.pos;
            float_t epseps = glm::dot( diff, diff );
            if ( epseps < value::forestProximityRunner )
            {
                // event::ProximityRunner
                forest.events.push( event::ProximityRunner( tick_next, &runner_a, &runner_b, epseps ) );
            }
        }

        // update trace for Runner
        runner_a.trace.pushIf( value::forestTraceD * value::forestTraceD, 
                               TracePoint( tick_next, glm::vec3( runner_a.move.aim[ 3 ] ) ) );
    }

    ////////////////////////////////////////////////////////////////////////////////
    // update tick of world
    //
    forest.tick += dt;

}

void StepDT::update(World& forest)
{
    // set respective Ogre based on Aim
    for (auto& runner : forest.runners )
    {
        runner.step();
    }
}

} // namespace forest

} // namespace batb



