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
#include "BATB/Keys/KeyMouseScroll.hpp"

namespace batb
{

namespace keys
{

KeyMouseScroll::KeyMouseScroll(BATB* b, code::MouseScroll c) : Key( b ), code_( c )
{
    auto unit = value::keyMouseScrollUnit;
    auto vel  = value::keyMouseScrollVel;

    follow_.speed( vel / unit );

}

void KeyMouseScroll::reset()
{
    follow_.clear();
}

void KeyMouseScroll::step(tick_t t)
{
    follow_.step( t );
}

float_t KeyMouseScroll::alpha()
{
    //std::cout << "scroll! current: " << follow_.get() << ", ideal: " << follow_.ideal << "\r" << std::flush;
    return follow_.get();
}

void KeyMouseScroll::add(double n)
{
    follow_.ideal += n / value::keyMouseScrollUnit;
}
////////////////////////////////////////////////////////////////////////////////
// names. default.

// unique identifier
std::string KeyMouseScroll::name() const
{
    std::ostringstream os; os << "KeyScrollAxis+";
    if ( code_ == code::MouseScroll::X ) os << "X";
    if ( code_ == code::MouseScroll::Y ) os << "Y";


    return os.str();
}
// human readable
std::string KeyMouseScroll::nameEasy() const
{
    return "Scroll wheel";
}

// GUI widget
std::string KeyMouseScroll::nameGUI() const
{
    std::ostringstream os; os << "<widget TBButton: text: '" << nameEasy() << "'>";
    return os.str();
}

} // namespace keys

} // namespace batb
