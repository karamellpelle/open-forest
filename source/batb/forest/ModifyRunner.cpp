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
#include "batb/forest/ModifyRunner.hpp"
#include "batb/forest/World.hpp"
#include "batb.hpp"
#include "batb/value/forest.hpp"


namespace batb
{

namespace forest
{

// 'dir' gets normalized!
void ModifyRunner::aim(const glm::vec2& dir)
{
    auto u = glm::normalize( dir );


    // World direction in xz plane
    aim_ = glm::mat4( -u.y, 0.0, u.x, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      u.x, 0.0, u.y, 0.0,
                      0.0, 0.0, 0.0, 1.0 );
    

}

// 'speed' gets clamped to [-1, 1]
void ModifyRunner::speed(float_t speed)
{
    speed_ = keep_inside( -1.0, 1.0, speed );
}

void ModifyRunner::runner(Runner* r)
{
    runner_ = r;

}

void ModifyRunner::operator()(World& forest)
{
    if ( runner_ )
    {
        const auto& pos = runner_->move.aim[3];

        // set intensity
        runner_->intensity = std::abs( speed_ );
        
        // direstion and position
        auto aim = glm::mat4( aim_[0],
                              aim_[1],
                              aim_[2],
                              pos
                            );

        // set aim of runner (this is horrible; DTMovable _has_ to be changed!)
        runner_->move.aim = aim;

        // set velocity, based on Terrain, 
        // TODO: later use Terrain more active, like running
        //       slow in heavy terrain
        float_t incline = std::min( 0.95, forest.terrain.incline( aim ) ); // prevent standing still on walls

        float_t speed = (1.0 - incline) * value::forestModifyRunnerSpeed // TODO: ensure 1.0 - 'incline' is OK
                        * speed_;

        runner_->move.vel[3] = (float)(speed) * aim[2]; // move along z-axis
       


    }

    // clear aim_ and speed_ here?
    
}

} // namespace forest

} // namespace batb



