//    orienteering-game: an orientering game.
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
#ifndef BATB_KEYS_KEYS_HPP
#define BATB_KEYS_KEYS_HPP
#include "batb/batb_include.hpp"
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

// Key's to be used to modify a world.
// typically ForestWorld, since RunWorld is typically controlled by GUI
class Keys
{
public:
    // create Key's, and let 'this' handle memory

    // prims
    KeyButton*      createKeyButton(KeyButton::Code code)             { return push_( new KeyButton( code ) ); }
    KeyMouseButton* createKeyMouseButton(KeyMouseButton::Code code)   { return push_( new KeyMouseButton( code ) ); }
    KeyMouseAxisX*  createKeyMouseAxisX()                             { return push_( new KeyMouseAxisX() ); }
    KeyMouseAxisY*  createKeyMouseAxisY()                             { return push_( new KeyMouseAxisY() ); }
    // cons
    KeyClicker*     createKeyClicker(Key* k)                          { return push_( new KeyClicker( k ) ); }
    KeyAlpha*       createKeyAlpha(Key* k)                            { return push_( new KeyAlpha( k ) ); }
    KeyPointer*     createKeyPointer(Key* x, Key* y, Key* l, Key* r)  { return push_( new KeyPointer( x, y, l, r ) ); }

    // invalidate and delete all keys
    void clear()
    {
        for (Container::iterator i = keys_.begin(); i != keys_.end(); ++i)
        {
            delete *i;
        }
        keys_.clear();
    }

    // clear all keys:
    void keys_clear()
    {
        for (Container::iterator i = keys_.begin(); i != keys_.end(); ++i)
        {
            (*i)->clear();
        }
    }

    // update all keys:
    void keys_update(tick_t t)
    {
        for (Container::iterator i = keys_.begin(); i != keys_.end(); ++i)
        {
            (*i)->update( t );
        }
    }

    static void charCalling(GLFWcharfun );
    static void keyCalling(GLFWkeyfun );
    static void mousebuttonCalling(GLFWmousebuttonfun );
    static void cursorposCalling(GLFWcursorposfun );
    static void scrollCalling(GLFWscrollfun );

private:
    typedef std::vector<Key*> Container;
    Container keys_;

    template <typename KeyPtr>
    KeyPtr push_(KeyPtr k)
    {
        keys_.push_back( k );
        return k;
    }

};


////////////////////////////////////////////////////////////////////////////////
//  

void begin(Keys& keys)
{

}

void end(Keys& keys)
{
    if ( keys.initialized_ )
    {

    }

    keys.initialized_ = false;
}

} // namespace keys

} // namespace batb


#endif
