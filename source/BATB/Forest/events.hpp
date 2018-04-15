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
#ifndef BATB_FOREST_EVENTS_HPP
#define BATB_FOREST_EVENTS_HPP
#include "include.hpp"


namespace batb
{
namespace forest
{

class Runner;
class Control;

namespace event
{


// Runner punched a Control
// single frame, typically
class ControlPunch
{
public:
    ControlPunch(tick_t t, Runner* r, Control* c) : tick( t ), runner( r ), control( c ) { }

    tick_t tick;
    Runner* runner = nullptr;
    Control* control = nullptr;

};


// Runner is almost in contact with Control
// multiple frames
class ProximityControl
{
public:
    ProximityControl(tick_t t, Control* c, Runner* r, float_t e = 0.0) : tick( t ), control( c ), runner( r ), epseps( e ) { }

    tick_t tick;
    Control* control = nullptr;
    Runner* runner = nullptr;
    float_t epseps = 0.0; // squared distance

    // TODO: moving in or out
};

// two Runner's are almost in contact
// multiple frames
class ProximityRunner
{
public:
    ProximityRunner(tick_t t, Runner* a, Runner* b, float_t e = 0.0) : tick( t ), runner_a( a ), runner_b( b ), epseps( e ) { }

    tick_t tick;
    Runner* runner_a = nullptr;
    Runner* runner_b = nullptr;
    float_t epseps = 0.0; // squared distance

    // TODO: moving in or out

};

} // namespace event
} // namespace forest
} // namespace batb



#endif

