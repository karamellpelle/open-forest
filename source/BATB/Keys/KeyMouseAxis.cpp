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
#include "BATB/Keys/KeyMouseAxis.hpp"

namespace batb
{

namespace keys
{

float_t KeyMouseAxis::alpha()
{
    double x, y;
    if ( can_disable )
        batb->keys->getCursorPos_( x, y );
    else
        batb->keys->getCursorPos( x, y ); 

    if ( code_ == code::MouseAxis::X ) return x;
    if ( code_ == code::MouseAxis::Y ) return y;
    return 0.0;
}

void KeyMouseAxis::set(float_t alpha) 
{
    double x, y;
    if ( can_disable )
    {
        batb->keys->getCursorPos_( x, y );
        if ( code_ == code::MouseAxis::X ) batb->keys->setCursorPos_( alpha, y ); 
        if ( code_ == code::MouseAxis::Y ) batb->keys->setCursorPos_( x, alpha ); 
    }
    else
    {
        batb->keys->getCursorPos( x, y ); 
        if ( code_ == code::MouseAxis::X ) batb->keys->setCursorPos( alpha, y ); 
        if ( code_ == code::MouseAxis::Y ) batb->keys->setCursorPos( x, alpha ); 
    }

}


////////////////////////////////////////////////////////////////////////////////
// names. default.

// unique identifier
std::string KeyMouseAxis::name() const
{
    std::ostringstream os; os << "KeyMouseAxis+";
    if ( code_ == code::MouseAxis::X ) os << "X";
    if ( code_ == code::MouseAxis::Y ) os << "Y";


    return os.str();
}
// human readable
std::string KeyMouseAxis::nameEasy() const
{
    return "Mouse";
}

// GUI widget
std::string KeyMouseAxis::nameGUI() const
{
    std::ostringstream os; os << "<widget TBButton: text: 'mouse'>"; // TODO: image
    return os.str();
}


} // namespace keys

} // namespace batb
