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
#include "BATB/Time.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Camera.hpp"
#include "BATB/Forest/AimKeysController.hpp"
#include "BATB/Value/Forest.hpp"
#include "glm/gtx/euler_angles.hpp"


namespace batb
{

namespace forest
{

AimKeysController::AimKeysController() 
{

}


void AimKeysController::step(BATB* batb)
{
    auto tick = batb->time->get();

    ////////////////////////////////////////////////////////////////////////////////
    // move camera in xy-plane from mouse (yaw, pitch)
    float_t x, y;
    batb->forest->keys->aim->axis( x, y );
    aim_a = value::forestAimX * (-x);
    aim_b = value::forestAimY * (y);

    auto rotation = glm::eulerAngleYXZ( aim_a, aim_b, aim_c );
    aim.aim[0] = rotation[0];
    aim.aim[1] = rotation[1];
    aim.aim[2] = rotation[2];


    ////////////////////////////////////////////////////////////////////////////////
    // move camera from keys. this should modify Runner

    // clear timed value
    if ( batb->forest->keys->left->pressed() || batb->forest->keys->right->pressed() )
    {
        tick_x = tick;
    }
    if ( batb->forest->keys->forward->pressed() || batb->forest->keys->backward->pressed() )
    {
        tick_z = tick;
    }

    float_t move_x = (batb->forest->keys->left->press()  ? (1.0)  : (0.0)) +
                     (batb->forest->keys->right->press() ? (-1.0) : (0.0));
    float_t move_z = (batb->forest->keys->forward->press()  ? (1.0)  : (0.0)) +
                     (batb->forest->keys->backward->press() ? (-1.0) : (0.0));

    // 'pos' part of Aim tells us press time
    aim.pos.x = move_x * (tick - tick_x);
    aim.pos.y = 0.0;
    aim.pos.z = move_z * (tick - tick_z);
    aim.pos.w = 1.0;

    // we work controllable_
    if ( controllable_ )
    {
        controllable_->aiming( aim );
    }
}





} // namespace forest

} // namespace batb



