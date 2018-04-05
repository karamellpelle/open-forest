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
#ifndef BATB_FOREST_MODIFYCONTROLCAMERA_HPP
#define BATB_FOREST_MODIFYCONTROLCAMERA_HPP
#include "batb/batb_include.hpp"

namespace batb
{


class BATB;


namespace forest
{

class DTMovable;
class World;
class ModifyCamera;

class ModifyControlCamera 
{
public:
    ModifyControlCamera(BATB& b) : batb( b )    { }

    void operator()(World& );

    void modifier(ModifyCamera* );

    // free (continuous movement)
    void free(DTMovable* , tick_t = 0.0);
    // follow at distance (continuous movement)
    void follow(DTMovable* , tick_t = 0.0);
    // connect to  (continuous movement)
    void attach(DTMovable* , tick_t = 0.0);

    ////////////////////////////////////////////////////////////////////////////////
    //
    BATB& batb;

private:
    ModifyCamera* modifier_ = nullptr;

    float_t aim_a_ = 0.0;
    float_t aim_b_ = 0.0;
    float_t aim_c_ = 0.0;

    DTMovable* following_ = nullptr;
    DTMovable* attached_ = nullptr;
};


} // namespace forest

} // namespace batb


#endif

