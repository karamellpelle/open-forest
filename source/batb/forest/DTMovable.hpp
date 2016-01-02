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
#ifndef BATB_FOREST_DTMOVABLE_HPP
#define BATB_FOREST_DTMOVABLE_HPP
#include "batb/forest/Aim.hpp"


namespace batb
{


namespace forest
{

////////////////////////////////////////////////////////////////////////////////
// this class should probably be changed to some better types for 
// physics, but I am not a physist so I don't know !
//
// z axis is forward
//
// TODO: dmat4 for better precision i a big world?

class DTMovable : public Aim
{
public:
    DTMovable() : Aim() { }

    //   should the 3x3 part of Aim be computed from quat_pos?
    //   like a function 'void compute()' updating all data (normalize)
    //   see: http://physicsforgames.blogspot.no/
    //
    //glm::quaternion quat_pos;
    //glm::quaternion quat_vel;
    //glm::quaternion quat_acc;

    glm::vec4  vel;      // velocity of 'Aim::Pos'
    glm::vec4  acc;      // acceleration of 'Aim::Pos'
    //glm::vec4 jerk;
   
    float_t speed = 0.0;

    // normalize all data
    void compute()
    {
        if ( !computed )
        {
            speed = glm::length( glm::vec3( vel ) );
            //aim = f( quat_pos );
            computed = true;
        }
    }

    bool computed = false;

};


} // namespace forest

} // namespace batb





#endif

