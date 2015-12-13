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
#include "batb/forest.hpp"
#include "batb.hpp"
#include "glm/gtx/euler_angles.hpp"


namespace batb
{

namespace forest
{


void ModifyControlCamera::operator()(World& forest)
{
    float_t x, y;
    batb.forest.keyset.aim->axis( x, y );
    bool press_l = batb.forest.keyset.aim->left()->press();
    bool press_r = batb.forest.keyset.aim->right()->press();

    constexpr tick_t aim_dt = 0.02;
    //aim_a = (-x) * 2.0;
    //aim_b = y * 2.0;
    //
    //////////////////////////////////////////
    //// set aim of runner from aim_x_
    //if ( aiming )
    //{
    //    aiming->aim = glm::eulerAngleYXZ( aim_a, aim_b, aim_c );
    //}
/*
    if ( forest.runners.empty() )
    {
        std::cout << "runners.empty!!\n"; 
        camera->setDirection( dir );
    }
    else
    {
        forest::Runner runner = forest.runners.front();
        glm::mat4 aim = runner.aim;
        glm::vec4 z = aim[2];
        
        camera->setDirection( Ogre::Vector3( z[0], z[1], z[2] ) );

        glm::vec4 pos = runner.pos;
        camera->setPosition( Ogre::Vector3( pos[0], pos[1], pos[2] ) );

    }
    */ 

}


} // namespace forest

} // namespace batb



