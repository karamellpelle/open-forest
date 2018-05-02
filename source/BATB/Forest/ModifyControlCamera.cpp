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
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Camera.hpp"
#include "BATB/Forest/ModifyControlCamera.hpp"
#include "BATB/Forest/ModifyCamera.hpp"
#include "BATB/Value/Forest.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "BATB/Run.hpp"
#include "BATB/Run/KeySet.hpp"

namespace batb
{

namespace forest
{

ModifyControlCamera::ModifyControlCamera(BATB* b) : batb( b ), x_speed_( value::forestCameraMoveXMax ), z_speed_( value::forestCameraMoveZMax )
{
    // magic numbers
    x_speed_.time( value::forestCameraMoveXTicks );
    z_speed_.time( value::forestCameraMoveZTicks );
}

void ModifyControlCamera::modifier(ModifyCamera* mod)
{
    modifier_ = mod;
}

void ModifyControlCamera::operator()(World& forest, tick_t tick)
{
    // TODO: move Camera by using the 'ModifyCamera' interface!

    Camera& camera = forest.camera;

    ////////////////////////////////////////////////////////////////////////////////
    // move camera in xy-plane from mouse (yaw, pitch)
    float_t x, y;
    batb->forest->keys->aim->axis( x, y );
    aim_a_ = value::forestAimX * (-x);
    aim_b_ = value::forestAimY * (y);
    auto aim = glm::eulerAngleYXZ( aim_a_, aim_b_, aim_c_ );
    camera.move.aim[0] = aim[0];
    camera.move.aim[1] = aim[1];
    camera.move.aim[2] = aim[2];
    // camera.move.aim[3] is position!



    ////////////////////////////////////////////////////////////////////////////////
    // move camera from keys. this should modify Runner

    // clear timed value
    if ( batb->forest->keys->left->pressed() || batb->forest->keys->right->pressed() )
    {
        x_speed_.clear( tick );
    }
    if ( batb->forest->keys->forward->pressed() || batb->forest->keys->backward->pressed() )
    {
        z_speed_.clear( tick );
    }

    float_t move_x = (batb->forest->keys->left->press()  ? (1.0)  : (0.0)) +
                     (batb->forest->keys->right->press() ? (-1.0) : (0.0));
    float_t move_z = (batb->forest->keys->forward->press()  ? (1.0)  : (0.0)) +
                     (batb->forest->keys->backward->press() ? (-1.0) : (0.0));

    float_t x_speed = std::max( value::forestCameraMoveXMin, x_speed_( tick ) ) * move_x; 
    float_t z_speed = std::max( value::forestCameraMoveZMin, z_speed_( tick ) ) * move_z; 

    // set 'vel' vector of DTMovable, by using direction 'aim' and x and z speed
    auto& vel = camera.move.vel;
    vel  = x_speed * aim[0] +    // x direction
           z_speed * aim[2];     // z direction

}


void ModifyControlCamera::free(DTMovable* move, tick_t time)
{
    attached_ = nullptr;
    following_ = nullptr;
}

void ModifyControlCamera::follow(DTMovable* move, tick_t time)
{
    if ( move )
    {
        following_ = move;

        // can not be attached and following at the same timd
        attached_ = nullptr;

    }
    else
    {
        // set free
    }
}

void ModifyControlCamera::attach(DTMovable* move, tick_t time)
{
    if ( move )
    {
        attached_ = move;

        // can not be attached and following at the same timd
        following_ = nullptr;
    }
    else
    {
        // set free
    }
}


} // namespace forest

} // namespace batb



