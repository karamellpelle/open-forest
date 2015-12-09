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


namespace batb
{

namespace forest
{

void StepDT::operator()(World& forest, tick_t dt)
{
/*
    // TODO: update runners, camera, ...
    float_t x, y;
    batb.forest.keyset.aim->axis( x, y );
    bool press_l = batb.forest.keyset.aim->left()->press();
    bool press_r = batb.forest.keyset.aim->right()->press();

    constexpr tick_t aim_dt = 0.02;
    aim_a = (-x) * 2.0;
    aim_b = y * 2.0;

    ////////////////////////////////////////
    // set aim of runner from aim_x_
    if ( aiming )
    {
        aiming->aim = glm::eulerAngleYXZ( aim_a, aim_b, aim_c );
    }


    ////////////////////////////////////////
    // * step dt of runner
    constexpr tick_t dt = 0.02;
    constexpr float_t press_speed = 100.0;

    tick_t tick_next = forest.run.tick;
    while ( forest.tick + dt <= tick_next )
    {
      
        // TODO: step 'aiming' (physics)
        if ( press_l || press_r )
        {
            glm::vec4 dir = aiming->aim[2];
            float alpha = ( press_r ? (-1.0) : (1.0) ) * dt * press_speed;
            aiming->pos += alpha * dir;
        }
        //
        forest.tick += dt;
    }

    glm::vec4 pos = aiming->pos;
    //std::cout << "\rcursor: " << x << " " << y << ", "
    //          << "pos: " << pos[0] << " "<< pos[1] << " "<< pos[2];
              
*/

}


} // namespace forest

} // namespace batb



