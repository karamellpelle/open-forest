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
#ifndef BATB_FOREST_AIMKEYSCONTROLLER_HPP
#define BATB_FOREST_AIMKEYSCONTROLLER_HPP
#include "BATB/Forest/AimControllable.hpp"

namespace batb
{
class BATB;


namespace forest
{


// using Keys to control an Aim
class AimKeysController 
{
public:
    AimKeysController();

    // eat input
    void step(BATB* );

    // what to control 
    void connect(AimControllable* );
    
    // set back pointer
    void recenter(BATB* );

    ////////////////////////////////////////////////////////////////////////////////
    //

    AimControllable::Aiming aiming;
    float_t aim_a = 0.0;
    float_t aim_b = 0.0;
    float_t aim_c = 0.0;

private:
    
    AimControllable* controllable_ = nullptr;

    tick_t tick_x_ = 0.0;
    tick_t tick_z_ = 0.0;
};


} // namespace forest

} // namespace batb


#endif

