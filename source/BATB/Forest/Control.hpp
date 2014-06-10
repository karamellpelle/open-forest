//    orienteering-game: an orientering game.
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
#ifndef BATB_CONTROL_HPP
#define BATB_CONTROL_HPP



namespace BATB
{


// each Control has a definition.
//
// a definition especially contains a code number. 
// 
// additionally, a definition contains placement information, like "north-west for
// object", size information, like "1.3m high", and other things.
// such information must be reflected in the Terrain. so, when we create a 
// Control in Terrain, then Terrain must write the corresponding ControlDefinition
// into Control.
class ControlDefinition
{
public:
    typedef uint Code;

    // code for this Control
    Code code() const
    {
        return code_;
    }

private:
    Code code_;
};


// Control
class Control
{
friend class Terrain;
public:

    // a Control always belong to a Terrain
    Control(Terrain* t) : terrain_( t )
    {
    }


    Terrain* terrain() const
    {
        return terrain_;
    }
    const ControlDefinition& definition() const
    {
        return definition_;
    }



    // should we define type of control, like normal, start, finish, etc?
    // no. i think we should not bind a control to be of a specific type, instead
    // just let a course define this. however, the GLAnimation for this Control
    // can be more specific for users, however, this is just a cosmetic detail.

private:

    // Control implies a Terrain, where this control is placed.
    Terrain* const terrain_;

    // definition for this Control
    ControlDefinition definition_;

    // statistic stuff:
    // * punches (player, time, ...)
    // ...

    // output props:
    // GLAnimation anim_; // regular, start point
};


}


#endif
