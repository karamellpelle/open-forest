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
#include "batb/forest/ModifyControlCamera.hpp"
#include "batb/forest/ModifyCamera.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb/forest/Camera.hpp"
#include "batb.hpp"
#include "batb/value/forest.hpp"
#include "glm/gtx/euler_angles.hpp"


namespace batb
{

namespace forest
{


void ModifyControlCamera::modifier(ModifyCamera* mod)
{
    modifier_ = mod;
}

void ModifyControlCamera::operator()(World& forest)
{
    Camera& camera = forest.camera;

    ////////////////////////////////////////////////////////////////////////////////
    // move camera in xy-plane from mouse (yaw, pitch)
    float_t x, y;
    batb.forest.keyset.aim->axis( x, y );
    aim_a_ = value::forestAimX * (-x);
    aim_b_ = value::forestAimY * (y);
    auto aim = glm::eulerAngleYXZ( aim_a_, aim_b_, aim_c_ );
    camera.move.aim[0] = aim[0];
    camera.move.aim[1] = aim[1];
    camera.move.aim[2] = aim[2];
    // camera.move.aim[3] is position!



    ////////////////////////////////////////////////////////////////////////////////
    // move camera from keys. this should modify Runner
    float_t move_x = (batb.forest.keyset.left->press()  ? (1.0)  : (0.0)) +
                     (batb.forest.keyset.right->press() ? (-1.0) : (0.0));
    float_t move_z = (batb.forest.keyset.forward->press()  ? (1.0)  : (0.0)) +
                     (batb.forest.keyset.backward->press() ? (-1.0) : (0.0));
    auto& vel = camera.move.vel[ 3 ];
    auto& x_ = aim[0];
    auto& z_ = aim[2];
    vel  = (move_x * value::forestMoveX) * x_ + 
           (move_z * value::forestMoveZ) * z_;
       
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



