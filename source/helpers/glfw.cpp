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
#include "helpers/glfw.hpp"
#include <GLFW/glfw3.h> 



int glfw_map_mousebutton(const std::string& name)
{
    if ( name.compare( "1" ) == 0)      return GLFW_MOUSE_BUTTON_1;
    if ( name.compare( "2" ) == 0)      return GLFW_MOUSE_BUTTON_2;
    if ( name.compare( "3" ) == 0)      return GLFW_MOUSE_BUTTON_3;
    if ( name.compare( "4" ) == 0)      return GLFW_MOUSE_BUTTON_4;
    if ( name.compare( "5" ) == 0)      return GLFW_MOUSE_BUTTON_5;
    if ( name.compare( "6" ) == 0)      return GLFW_MOUSE_BUTTON_6;
    if ( name.compare( "7" ) == 0)      return GLFW_MOUSE_BUTTON_7;
    if ( name.compare( "8" ) == 0)      return GLFW_MOUSE_BUTTON_8;
    if ( name.compare( "LAST" ) == 0)   return GLFW_MOUSE_BUTTON_LAST;
    if ( name.compare( "LEFT" ) == 0)   return GLFW_MOUSE_BUTTON_LEFT;
    if ( name.compare( "RIGHT" ) == 0)  return GLFW_MOUSE_BUTTON_RIGHT;
    if ( name.compare( "MIDDLE" ) == 0) return GLFW_MOUSE_BUTTON_MIDDLE;

    return GLFW_MOUSE_BUTTON_LAST;
}

int glfw_map_keyboardbutton(const std::string& name)
{
    if ( name.compare( "UNKNOWN" ) == 0 )      return GLFW_KEY_UNKNOWN;
    if ( name.compare( "SPACE" ) == 0 )        return GLFW_KEY_SPACE;
    if ( name.compare( "APOSTROPHE" ) == 0)    return GLFW_KEY_APOSTROPHE;
    if ( name.compare( "COMMA" ) == 0)         return GLFW_KEY_COMMA;
    if ( name.compare( "MINUS" ) == 0)         return GLFW_KEY_MINUS;
    if ( name.compare( "PERIOD" ) == 0)        return GLFW_KEY_PERIOD;
    if ( name.compare( "SLASH" ) == 0)         return GLFW_KEY_SLASH;
    if ( name.compare( "0" ) == 0)             return GLFW_KEY_0;
    if ( name.compare( "1" ) == 0)             return GLFW_KEY_1;
    if ( name.compare( "2" ) == 0)             return GLFW_KEY_2;
    if ( name.compare( "3" ) == 0)             return GLFW_KEY_3;
    if ( name.compare( "4" ) == 0)             return GLFW_KEY_4;
    if ( name.compare( "5" ) == 0)             return GLFW_KEY_5;
    if ( name.compare( "6" ) == 0)             return GLFW_KEY_6;
    if ( name.compare( "7" ) == 0)             return GLFW_KEY_7;
    if ( name.compare( "8" ) == 0)             return GLFW_KEY_8;
    if ( name.compare( "9" ) == 0)             return GLFW_KEY_9;
    if ( name.compare( "SEMICOLON" ) == 0)     return GLFW_KEY_SEMICOLON;
    if ( name.compare( "EQUAL" ) == 0)         return GLFW_KEY_EQUAL;
    if ( name.compare( "A" ) == 0)             return GLFW_KEY_A;
    if ( name.compare( "B" ) == 0)             return GLFW_KEY_B;
    if ( name.compare( "C" ) == 0)             return GLFW_KEY_C;
    if ( name.compare( "D" ) == 0)             return GLFW_KEY_D;
    if ( name.compare( "E" ) == 0)             return GLFW_KEY_E;
    if ( name.compare( "F" ) == 0)             return GLFW_KEY_F;
    if ( name.compare( "G" ) == 0)             return GLFW_KEY_G;
    if ( name.compare( "H" ) == 0)             return GLFW_KEY_H;
    if ( name.compare( "I" ) == 0)             return GLFW_KEY_I;
    if ( name.compare( "J" ) == 0)             return GLFW_KEY_J;
    if ( name.compare( "K" ) == 0)             return GLFW_KEY_K;
    if ( name.compare( "L" ) == 0)             return GLFW_KEY_L;
    if ( name.compare( "M" ) == 0)             return GLFW_KEY_M;
    if ( name.compare( "N" ) == 0)             return GLFW_KEY_N;
    if ( name.compare( "O" ) == 0)             return GLFW_KEY_O;
    if ( name.compare( "P" ) == 0)             return GLFW_KEY_P;
    if ( name.compare( "Q" ) == 0)             return GLFW_KEY_Q;
    if ( name.compare( "R" ) == 0)             return GLFW_KEY_R;
    if ( name.compare( "S" ) == 0)             return GLFW_KEY_S;
    if ( name.compare( "T" ) == 0)             return GLFW_KEY_T;
    if ( name.compare( "U" ) == 0)             return GLFW_KEY_U;
    if ( name.compare( "V" ) == 0)             return GLFW_KEY_V;
    if ( name.compare( "W" ) == 0)             return GLFW_KEY_W;
    if ( name.compare( "X" ) == 0)             return GLFW_KEY_X;
    if ( name.compare( "Y" ) == 0)             return GLFW_KEY_Y;
    if ( name.compare( "Z" ) == 0)             return GLFW_KEY_Z;
    if ( name.compare( "LEFT_BRACKET" ) == 0)  return GLFW_KEY_LEFT_BRACKET;
    if ( name.compare( "BACKSLASH" ) == 0)     return GLFW_KEY_BACKSLASH;
    if ( name.compare( "RIGHT_BRACKET" ) == 0) return GLFW_KEY_RIGHT_BRACKET;
    if ( name.compare( "ACCENT" ) == 0)        return GLFW_KEY_GRAVE_ACCENT;
    if ( name.compare( "WORLD_1" ) == 0)       return GLFW_KEY_WORLD_1;
    if ( name.compare( "WORLD_2" ) == 0)       return GLFW_KEY_WORLD_2;
    if ( name.compare( "ESCAPE" ) == 0)        return GLFW_KEY_ESCAPE;
    if ( name.compare( "ENTER" ) == 0)         return GLFW_KEY_ENTER;
    if ( name.compare( "TAB" ) == 0)           return GLFW_KEY_TAB;
    if ( name.compare( "BACKSPACE" ) == 0)     return GLFW_KEY_BACKSPACE;
    if ( name.compare( "INSERT" ) == 0)        return GLFW_KEY_INSERT;
    if ( name.compare( "DELETE" ) == 0)        return GLFW_KEY_DELETE;
    if ( name.compare( "RIGHT" ) == 0)         return GLFW_KEY_RIGHT;
    if ( name.compare( "LEFT" ) == 0)          return GLFW_KEY_LEFT;
    if ( name.compare( "DOWN" ) == 0)          return GLFW_KEY_DOWN;
    if ( name.compare( "UP" ) == 0)            return GLFW_KEY_UP;
    if ( name.compare( "PAGE_UP" ) == 0)       return GLFW_KEY_PAGE_UP;
    if ( name.compare( "PAGE_DOWN" ) == 0)     return GLFW_KEY_PAGE_DOWN;
    if ( name.compare( "HOME" ) == 0)          return GLFW_KEY_HOME;
    if ( name.compare( "END" ) == 0)           return GLFW_KEY_END;
    if ( name.compare( "CAPS_LOCK" ) == 0)     return GLFW_KEY_CAPS_LOCK;
    if ( name.compare( "SCROLL_LOCK" ) == 0)   return GLFW_KEY_SCROLL_LOCK;
    if ( name.compare( "NUM_LOCK" ) == 0)      return GLFW_KEY_NUM_LOCK;
    if ( name.compare( "PRINT_SCREEN" ) == 0)  return GLFW_KEY_PRINT_SCREEN;
    if ( name.compare( "PAUSE" ) == 0)         return GLFW_KEY_PAUSE;
    if ( name.compare( "F1" ) == 0)            return GLFW_KEY_F1;
    if ( name.compare( "F2" ) == 0)            return GLFW_KEY_F2;
    if ( name.compare( "F3" ) == 0)            return GLFW_KEY_F3;
    if ( name.compare( "F4" ) == 0)            return GLFW_KEY_F4;
    if ( name.compare( "F5" ) == 0)            return GLFW_KEY_F5;
    if ( name.compare( "F6" ) == 0)            return GLFW_KEY_F6;
    if ( name.compare( "F7" ) == 0)            return GLFW_KEY_F7;
    if ( name.compare( "F8" ) == 0)            return GLFW_KEY_F8;
    if ( name.compare( "F9" ) == 0)            return GLFW_KEY_F9;
    if ( name.compare( "F10" ) == 0)           return GLFW_KEY_F10;
    if ( name.compare( "F11" ) == 0)           return GLFW_KEY_F11;
    if ( name.compare( "F12" ) == 0)           return GLFW_KEY_F12;
    if ( name.compare( "F13" ) == 0)           return GLFW_KEY_F13;
    if ( name.compare( "F14" ) == 0)           return GLFW_KEY_F14;
    if ( name.compare( "F15" ) == 0)           return GLFW_KEY_F15;
    if ( name.compare( "F16" ) == 0)           return GLFW_KEY_F16;
    if ( name.compare( "F17" ) == 0)           return GLFW_KEY_F17;
    if ( name.compare( "F18" ) == 0)           return GLFW_KEY_F18;
    if ( name.compare( "F19" ) == 0)           return GLFW_KEY_F19;
    if ( name.compare( "F20" ) == 0)           return GLFW_KEY_F20;
    if ( name.compare( "F21" ) == 0)           return GLFW_KEY_F21;
    if ( name.compare( "F22" ) == 0)           return GLFW_KEY_F22;
    if ( name.compare( "F23" ) == 0)           return GLFW_KEY_F23;
    if ( name.compare( "F24" ) == 0)           return GLFW_KEY_F24;
    if ( name.compare( "F25" ) == 0)           return GLFW_KEY_F25;
    if ( name.compare( "KP_0" ) == 0)          return GLFW_KEY_KP_0;
    if ( name.compare( "KP_1" ) == 0)          return GLFW_KEY_KP_1;
    if ( name.compare( "KP_2" ) == 0)          return GLFW_KEY_KP_2;
    if ( name.compare( "KP_3" ) == 0)          return GLFW_KEY_KP_3;
    if ( name.compare( "KP_4" ) == 0)          return GLFW_KEY_KP_4;
    if ( name.compare( "KP_5" ) == 0)          return GLFW_KEY_KP_5;
    if ( name.compare( "KP_6" ) == 0)          return GLFW_KEY_KP_6;
    if ( name.compare( "KP_7" ) == 0)          return GLFW_KEY_KP_7;
    if ( name.compare( "KP_8" ) == 0)          return GLFW_KEY_KP_8;
    if ( name.compare( "KP_9" ) == 0)          return GLFW_KEY_KP_9;
    if ( name.compare( "KP_DECIMAL" ) == 0)    return GLFW_KEY_KP_DECIMAL;
    if ( name.compare( "KP_DIVIDE" ) == 0)     return GLFW_KEY_KP_DIVIDE;
    if ( name.compare( "KP_MULTIPLY" ) == 0)   return GLFW_KEY_KP_MULTIPLY;
    if ( name.compare( "KP_SUBTRACT" ) == 0)   return GLFW_KEY_KP_SUBTRACT;
    if ( name.compare( "KP_ADD" ) == 0)        return GLFW_KEY_KP_ADD;
    if ( name.compare( "KP_ENTER" ) == 0)      return GLFW_KEY_KP_ENTER;
    if ( name.compare( "KP_EQUAL" ) == 0)      return GLFW_KEY_KP_EQUAL;
    if ( name.compare( "LEFT_SHIFT" ) == 0)    return GLFW_KEY_LEFT_SHIFT;
    if ( name.compare( "LEFT_CONTROL" ) == 0)  return GLFW_KEY_LEFT_CONTROL;
    if ( name.compare( "LEFT_ALT" ) == 0)      return GLFW_KEY_LEFT_ALT;
    if ( name.compare( "LEFT_SUPER" ) == 0)    return GLFW_KEY_LEFT_SUPER;
    if ( name.compare( "RIGHT_SHIFT" ) == 0)   return GLFW_KEY_RIGHT_SHIFT;
    if ( name.compare( "RIGHT_CONTROL" ) == 0) return GLFW_KEY_RIGHT_CONTROL;
    if ( name.compare( "RIGHT_ALT" ) == 0)     return GLFW_KEY_RIGHT_ALT;
    if ( name.compare( "RIGHT_SUPER" ) == 0)   return GLFW_KEY_RIGHT_SUPER;
    if ( name.compare( "MENU" ) == 0)          return GLFW_KEY_MENU;
    if ( name.compare( "LAST" ) == 0)          return GLFW_KEY_LAST;

    return GLFW_KEY_UNKNOWN;

}


bool map_bool(const std::string& name, int& v)
{
    if ( name == "GLFW_FALSE" ) { v = GLFW_FALSE; return true; }
    if ( name == "false" ) { v = GLFW_FALSE; return true; }
    if ( name == "0" ) { v = GLFW_FALSE; return true; }
    if ( name == "GLFW_TRUE" ) { v = GLFW_TRUE; return true; }
    if ( name == "true" ) { v = GLFW_TRUE; return true; }
    if ( name == "1" ) { v = GLFW_TRUE; return true; }

    return false;
}

bool map_int(const std::string& name, int& v)
{
    try
    {
        v = std::stoul( name );
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool map_name(const std::string& name, int& v)
{
    if ( name == "GLFW_OPENGL_API" )             { v = GLFW_OPENGL_API; return true; }
    if ( name == "GLFW_OPENGL_ES_API" )          { v = GLFW_OPENGL_ES_API; return true; }
    if ( name == "GLFW_NO_API" )                 { v = GLFW_NO_API; return true; }
    if ( name == "GLFW_NATIVE_CONTEXT_API" )     { v = GLFW_NATIVE_CONTEXT_API; return true; }
    if ( name == "GLFW_EGL_CONTEXT_API" )        { v = GLFW_EGL_CONTEXT_API; return true; }
    if ( name == "GLFW_NO_ROBUSTNESS" )          { v = GLFW_NO_ROBUSTNESS; return true; }
    if ( name == "GLFW_NO_RESET_NOTIFICATION" )  { v = GLFW_NO_RESET_NOTIFICATION; return true; }
    if ( name == "GLFW_LOSE_CONTEXT_ON_RESET" )  { v = GLFW_LOSE_CONTEXT_ON_RESET; return true; }
    if ( name == "GLFW_ANY_RELEASE_BEHAVIOR" )   { v = GLFW_ANY_RELEASE_BEHAVIOR; return true; }
    if ( name == "GLFW_RELEASE_BEHAVIOR_FLUSH" ) { v = GLFW_RELEASE_BEHAVIOR_FLUSH; return true; }
    if ( name == "GLFW_RELEASE_BEHAVIOR_NONE" )  { v = GLFW_RELEASE_BEHAVIOR_NONE; return true; }
    if ( name == "GLFW_OPENGL_ANY_PROFILE" )     { v = GLFW_OPENGL_ANY_PROFILE; return true; }
    if ( name == "GLFW_OPENGL_COMPAT_PROFILE" )  { v = GLFW_OPENGL_COMPAT_PROFILE; return true; }
    if ( name == "GLFW_OPENGL_CORE_PROFILE" )    { v = GLFW_OPENGL_CORE_PROFILE; return true; }

    return false;
}


////////////////////////////////////////////////////////////////////////////////
// window creation hints. also returning default value. see
// http://www.glfw.org/docs/latest/window_guide.html#window_hints
//
bool glfw_set_windowhint(const std::string& name, const std::string& value)
{
    int v;
    if ( name.compare("GLFW_RESIZABLE") == 0 )                { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_RESIZABLE, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_VISIBLE") == 0 )                  { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_VISIBLE, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_DECORATED") == 0 )                { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_DECORATED, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_FOCUSED") == 0 )                  { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_FOCUSED, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_AUTO_ICONIFY") == 0 )             { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_AUTO_ICONIFY, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_FLOATING") == 0 )                 { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_FLOATING, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_MAXIMIZED") == 0 )                { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_MAXIMIZED, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_OPENGL_FORWARD_COMPAT") == 0 )    { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_OPENGL_DEBUG_CONTEXT") == 0 )     { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_STEREO") == 0 )                   { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_STEREO, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_SRGB_CAPABLE") == 0 )             { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_SRGB_CAPABLE, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE
    if ( name.compare("GLFW_DOUBLEBUFFER") == 0 )             { if ( map_bool( value, v ) ) { glfwWindowHint( GLFW_DOUBLEBUFFER, v ); return true; } else { return false; } } // GLFW_TRUE or GLFW_FALSE

    if ( name.compare("GLFW_RED_BITS") == 0 )                 { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_RED_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_GREEN_BITS") == 0 )               { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_GREEN_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_BLUE_BITS") == 0 )                { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_BLUE_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_ALPHA_BITS") == 0 )               { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_ALPHA_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_DEPTH_BITS") == 0 )               { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_DEPTH_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_STENCIL_BITS") == 0 )             { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_STENCIL_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_ACCUM_RED_BITS") == 0 )           { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_ACCUM_RED_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_ACCUM_GREEN_BITS") == 0 )         { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_ACCUM_GREEN_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_ACCUM_BLUE_BITS") == 0 )          { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_ACCUM_BLUE_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_ACCUM_ALPHA_BITS") == 0 )         { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_ACCUM_ALPHA_BITS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_AUX_BUFFERS") == 0 )              { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_AUX_BUFFERS, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_SAMPLES") == 0 )                  { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_SAMPLES, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_REFRESH_RATE") == 0 )             { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_REFRESH_RATE, v ); return true; } else { return false; } } // 0 to INT_MAX or GLFW_DONT_CARE
    if ( name.compare("GLFW_CONTEXT_VERSION_MAJOR") == 0 )    { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, v ); return true; } else { return false; } } // Any valid major version number of the chosen client API
    if ( name.compare("GLFW_CONTEXT_VERSION_MINOR") == 0 )    { if ( map_int( value, v ) ) { glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, v ); return true; } else { return false; } } // Any valid minor version number of the chosen client API

    if ( name.compare("GLFW_CLIENT_API") == 0 )               { if ( map_name( value, v ) ) { glfwWindowHint( GLFW_CLIENT_API, v ); return true; } else { return false; } } // GLFW_OPENGL_API, GLFW_OPENGL_ES_API or GLFW_NO_API
    if ( name.compare("GLFW_CONTEXT_CREATION_API") == 0 )     { if ( map_name( value, v ) ) { glfwWindowHint( GLFW_CONTEXT_CREATION_API, v ); return true; } else { return false; } } // GLFW_NATIVE_CONTEXT_API or GLFW_EGL_CONTEXT_API
    if ( name.compare("GLFW_CONTEXT_ROBUSTNESS") == 0 )       { if ( map_name( value, v ) ) { glfwWindowHint( GLFW_CONTEXT_ROBUSTNESS, v ); return true; } else { return false; } } // GLFW_NO_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION or GLFW_LOSE_CONTEXT_ON_RESET
    if ( name.compare("GLFW_CONTEXT_RELEASE_BEHAVIOR") == 0 ) { if ( map_name( value, v ) ) { glfwWindowHint( GLFW_CONTEXT_RELEASE_BEHAVIOR, v ); return true; } else { return false; } } // GLFW_ANY_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH or GLFW_RELEASE_BEHAVIOR_NONE
    if ( name.compare("GLFW_OPENGL_PROFILE") == 0 )           { if ( map_name( value, v ) ) { glfwWindowHint( GLFW_OPENGL_PROFILE, v ); return true; } else { return false; } } // GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE or GLFW_OPENGL_CORE_PROFILE

    return false;

}
