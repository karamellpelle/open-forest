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
#include "BATB/Keys/KeyKeyboardButton.hpp"

namespace batb
{

namespace keys
{
float_t KeyKeyboardButton::alpha()
{
    if ( can_disable ) return batb->keys->getKey_( code_ ) ? 1.0 : 0.0;
    
    return batb->keys->getKey( code_ ) ? 1.0 : 0.0; 
}
////////////////////////////////////////////////////////////////////////////////
// names. default.

// unique identifier
std::string KeyKeyboardButton::name() const
{
    // TODO: think through
    std::ostringstream os; os << "KeyKeyboardButton+" << code_;
    return os.str();
}
// human readable
std::string KeyKeyboardButton::nameEasy() const
{
// GLFW don't handle these properly!
    switch ( code_ )
    {
        case GLFW_KEY_SPACE:         return "Space";
        case GLFW_KEY_APOSTROPHE:    return "'";
        case GLFW_KEY_COMMA:         return ",";
        case GLFW_KEY_MINUS:         return "-";
        case GLFW_KEY_PERIOD:        return ".";
        case GLFW_KEY_SLASH:         return "/";
        case GLFW_KEY_SEMICOLON:     return ";";
        case GLFW_KEY_LEFT_BRACKET:  return "<";
        case GLFW_KEY_RIGHT_BRACKET: return ">";
        case GLFW_KEY_BACKSLASH:     return "\\";
        case GLFW_KEY_WORLD_1:       return "[World1]";
        case GLFW_KEY_WORLD_2:       return "[World2]";
        case GLFW_KEY_EQUAL         : return "=";
        case GLFW_KEY_ESCAPE        : return "Esc";
        case GLFW_KEY_ENTER         : return "Enter";
        case GLFW_KEY_TAB           : return "Tab";
        case GLFW_KEY_BACKSPACE     : return "Backspace";
        case GLFW_KEY_INSERT        : return "Ins";
        case GLFW_KEY_DELETE        : return "Del";
        case GLFW_KEY_RIGHT         : return "Right";
        case GLFW_KEY_LEFT          : return "Left";
        case GLFW_KEY_DOWN          : return "Down";
        case GLFW_KEY_UP            : return "Up";
        case GLFW_KEY_PAGE_UP       : return "PageUp";
        case GLFW_KEY_PAGE_DOWN     : return "PageDown";
        case GLFW_KEY_HOME          : return "Home";
        case GLFW_KEY_END           : return "End";
        case GLFW_KEY_CAPS_LOCK     : return "CapsLock";
        case GLFW_KEY_SCROLL_LOCK   : return "ScrollLock";
        case GLFW_KEY_NUM_LOCK      : return "NumLock";
        case GLFW_KEY_PRINT_SCREEN  : return "PrntScr";
        case GLFW_KEY_PAUSE         : return "Pause";
        case GLFW_KEY_F1            : return "F1";
        case GLFW_KEY_F2            : return "F2";
        case GLFW_KEY_F3            : return "F3";
        case GLFW_KEY_F4            : return "F4";
        case GLFW_KEY_F5            : return "F5";
        case GLFW_KEY_F6            : return "F6";
        case GLFW_KEY_F7            : return "F7";
        case GLFW_KEY_F8            : return "F8";
        case GLFW_KEY_F9            : return "F9";
        case GLFW_KEY_F10           : return "F10";
        case GLFW_KEY_F11           : return "F11";
        case GLFW_KEY_F12           : return "F12";
        //case GLFW_KEY_KP_DECIMAL    : return "KP ";
        //case GLFW_KEY_KP_DIVIDE     : return "";
        //case GLFW_KEY_KP_MULTIPLY   : return "";
        //case GLFW_KEY_KP_SUBTRACT   : return "";
        //case GLFW_KEY_KP_ADD        : return "";
        case GLFW_KEY_KP_ENTER      : return "KP Enter";
        //case GLFW_KEY_KP_EQUAL      : return "";
        case GLFW_KEY_LEFT_SHIFT    : return "Left Shift";
        case GLFW_KEY_LEFT_CONTROL  : return "Left Ctrl";
        case GLFW_KEY_LEFT_ALT      : return "Left Alt";
        case GLFW_KEY_LEFT_SUPER    : return "Left Super";
        case GLFW_KEY_RIGHT_SHIFT   : return "Right Shift";
        case GLFW_KEY_RIGHT_CONTROL : return "Right Ctrl";
        case GLFW_KEY_RIGHT_ALT     : return "Right Alt";
        case GLFW_KEY_RIGHT_SUPER   : return "Right Super";
        case GLFW_KEY_MENU          : return "Menu";
    }

    auto n = glfwGetKeyName( code_, 0 );
    return n == nullptr ? "(unknown keyboard button)" : std::string( n );

}

// GUI widget
std::string KeyKeyboardButton::nameGUI() const
{
    // TODO: custom skin to show key instead of a button
    std::ostringstream os; os << "<widget TBButton: skin: Key.keyboard, text: '" << nameEasy() << "'>";
    return os.str();
}

} // namespace keys

} // namespace batb
