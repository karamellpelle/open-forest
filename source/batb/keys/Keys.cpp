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
#include "batb/keys/Keys.hpp"
#include "batb/keys/KeyButton.hpp"
#include "batb/keys/KeyMouseButton.hpp"
#include "batb/keys/KeyMouseAxis.hpp"
#include "batb/keys/KeyClicker.hpp"
#include "batb/keys/KeyAlpha.hpp"
#include "batb/keys/KeyPointer.hpp"



namespace batb
{

namespace keys
{


////////////////////////////////////////////////////////////////////////////////
//

void Keys::clear()
{
    std::for_each( std::begin( keys_ ), std::end( keys_ ), std::default_delete<Key>() );
    keys_.clear();
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
    glfwSetScrollCallback( window_, f );
}


////////////////////////////////////////////////////////////////////////////////
// create Key's
// 

KeyButton* Keys::createKeyButton(int code)
{
    return push( new KeyButton( *this, code ) );
}

KeyMouseButton* Keys::createKeyMouseButton(int code)
{
    return push( new KeyMouseButton( *this, code ) );
}

KeyMouseAxisX* Keys::createKeyMouseAxisX()
{
    return push( new KeyMouseAxisX( *this ) );
}

KeyMouseAxisY* Keys::createKeyMouseAxisY()
{
    return push( new KeyMouseAxisY( *this ) );
}

KeyClicker* Keys::createKeyClicker(Key* k)
{
    return push( new KeyClicker( *this, k ) );
}

KeyAlpha* Keys::createKeyAlpha(Key* k)
{
    return push( new KeyAlpha( *this, k ) );
}

KeyPointer* Keys::createKeyPointer(Key* x, Key* y, Key* l, Key* r)
{
    return push( new KeyPointer( *this, x, y, l, r ) );
}

KeyPointer* Keys::createKeyPointer()
{
    return createKeyPointer( createKeyMouseAxisX(), createKeyMouseAxisY(), createKeyMouseButton( KeyMouseButton::left ), createKeyMouseButton( KeyMouseButton::right ) );
}

Key* Keys::createKey(const YAML::Node& yaml)
{
    // TODO!
    return nullptr;
}



////////////////////////////////////////////////////////////////////////////////
//  


void begin(Keys& keys)
{
    keys.window_ = env::screen_window(); // TODO: batb.env.window

    // cursor input mode is _NORMAL!
    glfwSetInputMode( keys.window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL ); 
    keys.cursor_free_ = false;

    //
    keys.initialized_ = true;
}

void end(Keys& keys)
{
    if ( keys.initialized_ )
    {
        keys.clear();
    }

    keys.initialized_ = false;
}

} // namespace keys

} // namespace batb

