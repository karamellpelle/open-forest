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
#ifndef BATB_FOREST_CONTROL_HPP
#define BATB_FOREST_CONTROL_HPP
#include "BATB/Forest/Aim.hpp"

namespace Ogre
{
class Entity;
}


namespace batb
{


namespace forest
{

class World;
class Runner;

// each Control has a definition.
//
// a definition especially contains a code number. 
// 
// additionally, a definition contains placement information, like "north-west of
// object", size information, like "1.3m high", type, and other things.
// such information must be reflected in the Terrain. so, when we create a 
// Control in World, then World must use the corresponding ControlDefinition (for 
// example a suitable 3D model)
class ControlDefinition
{
public:
    using Code = uint;
    enum class Type { Empty, Start, Normal, Finish, /*etc*/ };

    ControlDefinition() { }
    ControlDefinition(float_t x_, float_t z_, Code c = 0, Type t = Type::Empty) : x( x_ ), z( z_ ), code( c ), type( t ) { } 


    float_t x = 0.0;
    float_t z = 0.0;
    Code code;

    Type type;

    // for  placement
    // N,S,W,E,NW,NE,SW,SE
    // ...
};


////////////////////////////////////////////////////////////////////////////////
// Control
//
// should we have different classes of controls for start, finish, normal, etc?
// no. i think we should have only one control type, 'Control', and
// let definition and output (Ogre, AL) define their type. then a external
// Course (of a superworld of forest::World) interpret Control's as wanted type.

class Control
{
public:
    Control(World& w) : forest( w )   { }

    // assignment/copy allowed, if object copy, create 'clone()'
    Control(const Control& ) = default;
    Control& operator=(const Control& ) = default;
    
    ////////////////////////////////////////////////////////////////////////////////
    void reset(const ControlDefinition& def); // create or replace a Control from definition in Terrain
    void reset();                             // remove a Control from Terrain
    void punch(Runner* );

    ////////////////////////////////////////////////////////////////////////////////
    World& forest;
    ControlDefinition definition;

    // placement in World
    Aim aim;

    ////////////////////////////////////////////////////////////////////////////////
    // output

    Ogre::Entity* ogre_entity = nullptr;
    //ALuint al_source_;

    ////////////////////////////////////////////////////////////////////////////////
    // statistic
    uint stats_punches = 0;
    // TODO: list of Punch (player, time, ...)
    // ...

};


} // namespace forest

} // namespace batb





#endif
