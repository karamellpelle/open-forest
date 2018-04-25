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
#ifndef BATB_KEYS_KEY_BUTTON_HPP
#define BATB_KEYS_KEY_BUTTON_HPP
#include "BATB/Keys/Key.hpp"

namespace batb
{


namespace keys
{

// a keyboard button
class KeyKeyboardButton : public Key
{
public:
    KeyKeyboardButton(BATB* b, code::KeyboardButton c) : Key( b ), code_( c )     { } 

    void reset() override         { }
    void step(tick_t ) override   { }
    float_t alpha() override;

    // keyboard button names
    virtual std::string name()     const override; // unique identifier
    virtual std::string nameEasy() const override; // human readable
    virtual std::string nameGUI()  const override; // GUI widget

private:
    code::KeyboardButton code_;

};



} // namespace keys

} // namespace batb
#endif
