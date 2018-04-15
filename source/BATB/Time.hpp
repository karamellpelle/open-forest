//    open-forest: an orienteering game.
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
#ifndef BATB_TIME_HPP
#define BATB_TIME_HPP
#include "BATB/ModuleBATB.hpp"
#include "BATB/Screen.hpp"


namespace batb
{
namespace time
{

class Time : public ModuleBATB
{
public:
    Time(BATB* b): ModuleBATB( b ) { }

    ////////////////////////////////////////////////////////////////
    // setup
    void begin(const std::string& );
    void end();

    // frame
    void frameBegin();
    void frameEnd();


    ////////////////////////////////////////////////////////////////
    // tick
    tick_t get() const                             { return tick_; }           // tick of current frame
    tick_t getNow() const                          { return glfwGetTime(); }   // current tick

private:
    tick_t tick_ = 0.0;

};


} // namespace time
} // namespace batb


#endif

