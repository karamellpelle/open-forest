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
#ifndef BATB_KEYS_KEY_MOUSE_BUTTON_HPP
#define BATB_KEYS_KEY_MOUSE_BUTTON_HPP
#include "BATB/Keys/Key.hpp"

namespace batb
{


namespace keys
{

class KeyMouseButton : public Key
{
public:
    KeyMouseButton(Keys* keys, int c) : Key( keys ), code_( c )   { } 

    void reset() override                 { }
    void step(tick_t ) override           { }
    float_t alpha() override              { if ( can_disable ) return (keys->getMouseButton_( code_ ) ? 1.0 : 0.0);
                                                          else return (keys->getMouseButton( code_ )  ? 1.0 : 0.0); } 
    
    static constexpr int left   = GLFW_MOUSE_BUTTON_LEFT;
    static constexpr int right  = GLFW_MOUSE_BUTTON_RIGHT;

private:

    int code_ = 0;
};




} // namespace keys

} // namespace batb

#endif
