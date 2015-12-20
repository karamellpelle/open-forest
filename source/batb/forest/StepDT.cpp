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
#include "batb/forest/StepDT.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb/forest/events.hpp"
#include "batb.hpp"
#include "batb/value/forest.hpp"


namespace batb
{

namespace forest
{


void stepdt(DTMovable& move, tick_t dt)
{
    // currently, only the position part (mat4x4[ 3 ]) is updated
    auto& pos = move.aim[ 3 ];
    auto& vel = move.vel[ 3 ];
    auto& acc = move.acc[ 3 ];
    
    acc[3] = 1.0;

    vel += (float)( dt ) * acc;
    vel[3] = 1.0;

    pos += (float)( dt ) * vel;
    pos[3] = 1.0;

    //move.aim[3] = pos;
    //move.vel[3] = vel;
    //move.acc[3] = acc;

}


void StepDT::operator()(World& forest, tick_t dt)
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
        auto& pos = runner_a.move.aim[3];
        pos.y = 14 + forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( 
                     Ogre::Vector3( pos.x, pos.y, pos.z ) );

        // collide with controls
        for (auto j = std::begin( forest.controls ); j != std::end( forest.controls ); ++j)
        {
            auto& control = *j;

            // see if runner is close to control
            auto diff = runner_a.move.aim[3] - control.aim[3];
            float_t epseps = glm::dot( diff, diff );
            if ( epseps < value::forestProximityControl )
            {
                // event::ProximityControl
                forest.events.push( event::ProximityControl( &control, &runner_a, epseps ) );
            }
        }

        // collide with other runners
        auto j = i;
        for (++j; j != std::end( forest.runners ); ++j)
        {
            auto& runner_b = *j;

            // see if runner is close to control
            auto diff = runner_a.move.aim[3] - runner_b.move.aim[3];
            float_t epseps = glm::dot( diff, diff );
            if ( epseps < value::forestProximityRunner )
            {
                // event::ProximityRunner
                forest.events.push( event::ProximityRunner( &runner_a, &runner_b, epseps ) );
            }
        }

        // update trace for Runner
        runner_a.trace.pushIf( value::forestTraceD * value::forestTraceD, 
                               TracePoint( tick_next, glm::vec3( runner_a.move.aim[ 3 ] ) ) );
    }

    // NOTE:
    // tick of forest::World is not updated here!
}


} // namespace forest

} // namespace batb



