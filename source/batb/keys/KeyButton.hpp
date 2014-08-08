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
#ifndef BATB_KEYS_KEY_BUTTON_HPP
#define BATB_KEYS_KEY_BUTTON_HPP
#include "batb/keys/Key.hpp"

namespace batb
{


namespace keys
{

class KeyButton : public Key
{
friend class KeySet;

public:
    void clear() override             { }
    void update(tick_t ) override     { }
    float_t alpha() override          { return glfwGetKey( env::screen_window(), code_ ) ? 1.0 : 0.0; }

    typedef int Code;

    void code(Code c)                 { code_ = c; } 
    void load(/*def*/)             { } 

private:
    KeyButton() : code_( 0 )          { } // FIXME: GLFW_???
    KeyButton(Code c) : code_( c )    { } 
    Code code_;

};


} // namespace keys

} // namespace batb
#endif
