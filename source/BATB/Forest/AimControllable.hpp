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
#ifndef BATB_FOREST_AIMCONTROLLABLE_HPP
#define BATB_FOREST_AIMCONTROLLABLE_HPP
#include "BATB/Forest/Aim.hpp"


namespace batb
{
namespace forest
{


// something that can be controlled by Aim (i.e. AimKeysController)
class AimControllable
{
public:
    class Aiming : public Aim
    {
    public:
        float_t move_x = 0.0;
        float_t move_z = 0.0;

        tick_t ticks_x = 0.0;
        tick_t ticks_z = 0.0;
    };

    // we may later create a class Aiming containing more info
    virtual void aiming(const Aiming& ) = 0;

};



} // namespace forest

} // namespace batb



#endif
