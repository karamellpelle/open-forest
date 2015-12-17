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
#ifndef BATB_FOREST_CONTROL_HPP
#define BATB_FOREST_CONTROL_HPP
#include "batb/batb_include.hpp"

namespace Ogre
{
class Entity;
}


namespace batb
{

class BATB;

namespace forest
{

class World;
class Runner;

// each Control has a definition.
//
// a definition especially contains a code number. 
// 
// additionally, a definition contains placement information, like "north-west for
// object", size information, like "1.3m high", and other things.
// such information must be reflected in the Terrain. so, when we create a 
// Control in Terrain, then Terrain must write the corresponding ControlDefinition
// into Control. ??
class ControlDefinition
{
public:
    using Code = uint;
    ControlDefinition() { }
    ControlDefinition(float_t x_, float_t z_, Code c = 0) : x( x_ ), z( z_ ), code( c ) { } 


    float_t x = 0.0;
    float_t z = 0.0;
    Code code;

    // for  placement and output
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
    Control(World& w) : forest( w ) { }

    // assignment/copy allowed, if object copy, create 'clone()'
    Control(const Control& ) = default;
    Control& operator=(const Control& ) = default;
    
    ////////////////////////////////////////////////////////////////////////////////
    void reset(const ControlDefinition& def);
    void punch(Runner* );

    ////////////////////////////////////////////////////////////////////////////////
    World& forest;
    ControlDefinition definition;
    glm::mat4 aim; // position and aim. TODO: glm::dvec4?

    ////////////////////////////////////////////////////////////////////////////////
    // output

    Ogre::Entity* ogre_entity;
    //ALuint al_source_;

    ////////////////////////////////////////////////////////////////////////////////
    // statistic
    uint stats_punches;
    // TODO: list of Punch (player, time, ...)
    // ...

};


} // namespace forest

} // namespace batb





#endif
