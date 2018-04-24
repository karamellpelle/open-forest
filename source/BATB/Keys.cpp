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
#include "BATB/Keys.hpp"
#include "BATB/Keys/KeyKeyboardButton.hpp"
#include "BATB/Keys/KeyMouseButton.hpp"
#include "BATB/Keys/KeyMouseAxis.hpp"
#include "BATB/Keys/KeyMouseScroll.hpp"
#include "BATB/Keys/KeyClicker.hpp"
#include "BATB/Keys/KeyAlpha.hpp"
#include "BATB/Keys/KeyPointer.hpp"
#include "BATB/Screen.hpp"



namespace batb
{

namespace keys
{

// the kananical Keys (needed for scroll)
Keys* Keys::keysobj_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
// setup 

void Keys::begin(const std::string& path)
{
    batb->log << "batb->keys->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
        // set configuration file
        config( path );

        // retrieve GLFW window (needed for input)
        window_ = batb->screen->glfw_window;

        // cursor input mode is _NORMAL!
        glfwSetInputMode( window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL ); 
        cursor_free_ = false;

        // let scroll callback be able to reach this object (implies only 1 Keys in program)
        keysobj_ = this;
        glfwSetScrollCallback( window_, scroll_callback_ );
    }

    init( true );
}

void Keys::end()
{
    batb->log << "batb->keys->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        // save configuration
        save();

        // remove all created Key's
        clear();
    }

    init( false );
}
////////////////////////////////////////////////////////////////////////////////
//

// remove all created Key's 
void Keys::clear()
{
    std::for_each( std::begin( keys_ ), std::end( keys_ ), std::default_delete<Key>() );
    keys_.clear();

    // also clear scroll's
    scrolls_x_.clear();
    scrolls_y_.clear();
}


// reset all Key's
void Keys::reset()
{
    std::for_each( std::begin( keys_ ), std::end( keys_ ), [](auto k) { k->reset(); } );
}

// update all keys, to be done at each frame:
void Keys::step(tick_t t)
{
    std::for_each( std::begin( keys_ ), std::end( keys_ ), [t](auto k) { k->step( t ); } );
}



////////////////////////////////////////////////////////////////////////////////
//

void Keys::keyEnable(bool enable)
{
    if ( key_enable_ != enable )
    {
        if ( enable )
        {
            glfwSetCursorPos( window_, cursor_enable_x0_, cursor_enable_y0_ );
        }
        else
        {
            // save current state (i.e. cursor pos)
            double x1, y1;
            glfwGetCursorPos( window_, &x1, &y1 );

            cursor_enable_x0_ = x1;
            cursor_enable_y0_ = y1;

            // also previous cursor pos
            cursor_x0_ = x1;
            cursor_y0_ = y1;
        }
    }

    key_enable_ = enable;
}


////////////////////////////////////////////////////////////////////////////////
//


// get cursor pos, in pixels
void Keys::getCursorPos(uint& x, uint& y)
{
    // TODO: use env instead of GLFW?
    double x_, y_;
    glfwGetCursorPos( window_, &x_, &y_ );
    x = (uint)( x_ );
    y = (uint)( y_ );
}

// get cursor pos, relative to screen shape 
void Keys::getCursorPos(double& x, double& y)
{
    // TODO: use env instead of GLFW?
    int wth; int hth;
    glfwGetFramebufferSize( window_, &wth, &hth );
    float_t scale = 1.0 / (float_t)( std::max( wth, hth ) );

    double x_, y_;
    glfwGetCursorPos( window_, &x_, &y_ );
    x = (float_t)( x_ ) * scale;
    y = (float_t)( y_ ) * scale;
}


void Keys::getCursorPos_(uint& x, uint& y)
{
    if ( key_enable_ )
    {
        getCursorPos( x, y );
    }
    else
    {
        x = (uint)( cursor_enable_x0_ );
        y = (uint)( cursor_enable_y0_ );
    }
}

void Keys::getCursorPos_(double& x, double& y)
{
    if ( key_enable_ )
    {
        glfwGetCursorPos( window_, &x, &y );
    }
    else
    {
        x = cursor_enable_x0_;
        y = cursor_enable_y0_;
    }

    int wth; int hth;
    glfwGetFramebufferSize( window_, &wth, &hth );
    double scale = 1.0 / (double)( std::max( wth, hth ) );
    x *=  scale;
    y *=  scale;
}


void Keys::setCursorFree(bool free)
{
    if ( cursor_free_ != free )
    {
        // get current
        double x1, y1;
        glfwGetCursorPos( window_, &x1, &y1 );

        
        glfwSetInputMode( window_, GLFW_CURSOR, 
            free ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL ); 
            // ^ FIXME: _NORMAL/_HIDDEN based on setting!

        // set back to previous
        glfwSetCursorPos( window_, cursor_x0_, cursor_y0_ );

        // save previous
        cursor_x0_ = x1;
        cursor_y0_ = y1;

    }
    cursor_free_ = free;
}

void Keys::charCalling(GLFWcharfun f)
{
    glfwSetCharCallback( window_, f );
}

void Keys::keyCalling(GLFWkeyfun f)
{
    glfwSetKeyCallback( window_, f );
}

void Keys::mousebuttonCalling(GLFWmousebuttonfun f)
{
    glfwSetMouseButtonCallback( window_, f );
}

void Keys::cursorposCalling(GLFWcursorposfun f)
{
    glfwSetCursorPosCallback( window_, f );
}

void Keys::scrollCalling(GLFWscrollfun f)
{
    scroll_cbks_.push_back( f );
}


////////////////////////////////////////////////////////////////////////////////
// create Key's
// 
// TODO: look see the same exist!

KeyKeyboardButton* Keys::createKeyKeyboardButton(int code)
{
    return push( new KeyKeyboardButton( this, code ) );
}

KeyMouseButton* Keys::createKeyMouseButton(int code)
{
    return push( new KeyMouseButton( this, code ) );
}

KeyMouseAxisX* Keys::createKeyMouseAxisX()
{
    return push( new KeyMouseAxisX( this ) );
}

KeyMouseAxisY* Keys::createKeyMouseAxisY()
{
    return push( new KeyMouseAxisY( this ) );
}

KeyMouseScroll* Keys::createKeyMouseScrollY()
{
    auto ret = push( new KeyMouseScroll( this ) );

    // also add to scrolls
    scrolls_y_.push_back( ret );
    return ret;
}

KeyMouseScroll* Keys::createKeyMouseScrollX()
{
    auto ret = push( new KeyMouseScroll( this ) );

    // also add to scrolls
    scrolls_x_.push_back( ret );
    return ret;
}


KeyClicker* Keys::createKeyClicker(Key* k)
{
    return push( new KeyClicker( this, k ) );
}

KeyAlpha* Keys::createKeyAlpha(Key* k)
{
    return push( new KeyAlpha( this, k ) );
}

KeyPointer* Keys::createKeyPointer(Key* x, Key* y, Key* l, Key* r)
{
    return push( new KeyPointer( this, x, y, l, r ) );
}

KeyPointer* Keys::createKeyPointer()
{
    return createKeyPointer( createKeyMouseAxisX(), createKeyMouseAxisY(), createKeyMouseButton( KeyMouseButton::Left ), createKeyMouseButton( KeyMouseButton::Right ) );
}

Key* Keys::createKey(const YAML::Node& yaml)
{
    using namespace YAML;
   
    // which kind of Key?
    if ( Node key = yaml["KeyKeyboardButton"] )
    {
        int map_code_str(const std::string& ); // this creates glfw keys (int) from string

        auto code_str = key["code"].as<std::string>( "" );
        if ( code_str.empty() )
        {
            batb->log << "ERROR: no 'code' field"; 
            return nullptr;
        }
        
        auto glfw_code =  map_code_str( code_str ) ;
        auto ret = createKeyKeyboardButton( glfw_code );
        batb->log << "KeyKeyboardButton: { code: ";
        if   ( glfw_code == GLFW_KEY_UNKNOWN ) batb->log << "UNKNOWN (" << code_str << ")"; // maybe this should be ERROR instead?
        else                                   batb->log << code_str;
        batb->log << " }";

        return ret;
    }

    batb->log << "(key not known)"; // TODO: more description
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
//
void Keys::scroll_callback_(GLFWwindow* win, double x, double y)
{
    
    //std::cout << "scroll! " << x << ", " << y << std::endl;
    
    // callback every registered callback
    for ( auto cbk : Keys::keysobj_->scroll_cbks_ )
    {
        cbk( win, x, y );
    }

    // call our KeyMouseScroll's
    for ( auto scroll : Keys::keysobj_->scrolls_x_ )
    {
       // TODO: if enabled  
       scroll->add( x );
    }
    for ( auto scroll : Keys::keysobj_->scrolls_y_ )
    {
       // TODO: if enabled  
       scroll->add( y );
    }

}

// map names into GLFW keyboar key
int map_code_str(const std::string& name)
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

} // namespace keys

} // namespace batb

