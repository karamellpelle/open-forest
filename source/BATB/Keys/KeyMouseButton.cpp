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
#include "BATB/Keys/KeyMouseButton.hpp"

namespace batb
{

namespace keys
{
////////////////////////////////////////////////////////////////////////////////
// names. default.

// unique identifier
std::string KeyMouseButton::name() const
{
    // TODO: think through
    std::ostringstream os; os << "KeyMouseButton+" << code_;
    return os.str();
}
// human readable
std::string KeyMouseButton::nameEasy() const
{
    if ( code_ == GLFW_MOUSE_BUTTON_LEFT )   return "Left click";
    if ( code_ == GLFW_MOUSE_BUTTON_MIDDLE ) return "Middle click";
    if ( code_ == GLFW_MOUSE_BUTTON_RIGHT )  return "Right click";
    
    // some of these are overlapping above
    if ( code_ == GLFW_MOUSE_BUTTON_1 )      return "mouse button 1";
    if ( code_ == GLFW_MOUSE_BUTTON_2 )      return "mouse button 2";
    if ( code_ == GLFW_MOUSE_BUTTON_3 )      return "mouse button 3";
    if ( code_ == GLFW_MOUSE_BUTTON_4 )      return "mouse button 4";
    if ( code_ == GLFW_MOUSE_BUTTON_5 )      return "mouse button 5";
    if ( code_ == GLFW_MOUSE_BUTTON_6 )      return "mouse button 6";
    if ( code_ == GLFW_MOUSE_BUTTON_7 )      return "mouse button 7";
    if ( code_ == GLFW_MOUSE_BUTTON_8 )      return "mouse button 8";

    return "(unknown mouse button)";
}

// GUI widget
std::string KeyMouseButton::nameGUI() const
{
    // TODO: icons for every mouse button:
    // "<widget TBButton: skin: Key.mousebutton-right'" << nameEasy() << "'>";
    std::ostringstream os; os << "<widget TBButton: text: '" << nameEasy() << "'>";
    return os.str();
}

} // namespace keys

} // namespace batb
