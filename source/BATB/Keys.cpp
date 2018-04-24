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
    // TODO!
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



} // namespace keys

} // namespace batb

