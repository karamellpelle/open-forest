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
#include "batb/batb_include.hpp"


namespace batb
{


namespace forest
{

////////////////////////////////////////////////////////////////////////////////
// this class should probably be changed to some better types for 
// physics, but I am not a physist so I don't know !
//
// for education: http://physicsforgames.blogspot.no/
//
// z axis is forward
class DTMovable 
{
public:
    // TODO: dmat4 for better precision i a big world?
    //
    glm::mat4 aim;      // position and orientation  
    glm::mat4 vel;      // velocity of 'aim'
    glm::mat4 acc;      // acceleration of 'aim' (velocity of 'vel')
    //glm::vec4 jerk;


};


} // namespace forest

} // namespace batb





#endif

