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
#include "batb.hpp"


namespace batb
{

namespace forest
{

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

    // currently, only the position part (mat4x4[ 3 ]) is updated
    auto& pos = camera.move.aim[ 3 ];
    auto& vel = camera.move.vel[ 3 ];
    auto& acc = camera.move.acc[ 3 ];
    
    acc[3] = 1.0;

    vel += (float)( dt ) * acc;
    vel[3] = 1.0;

    pos += (float)( dt ) * vel;
    pos[3] = 1.0;

    camera.move.aim[3] = pos;
    camera.move.vel[3] = vel;
    camera.move.acc[3] = acc;
    
    ////////////////////////////////////////////////////////////////////////////////
    // step runners
    // 
   
    // 
}


} // namespace forest

} // namespace batb



