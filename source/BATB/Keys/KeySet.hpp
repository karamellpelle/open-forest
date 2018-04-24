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
#ifndef BATB_KEYS_KEYSET_HPP
#define BATB_KEYS_KEYSET_HPP
#include "BATB/Keys/KeyKeyboardButton.hpp"
#include "BATB/Keys/KeyMouseButton.hpp"
#include "BATB/Keys/KeyMouseAxis.hpp"
#include "BATB/Keys/KeyMouseScroll.hpp"
#include "BATB/Keys/KeyClicker.hpp"
#include "BATB/Keys/KeyAlpha.hpp"
#include "BATB/Keys/KeyPointer.hpp"


namespace batb
{

namespace keys
{

class Keys;

// create Key's from bound Keys
// 
// should typically be subclassed into a class having members
// of each Key necessary. this subclass should typically have
// a method 'load(const std::string& filepath)' to load
// a definition from a config (YAML) file.
class KeySet
{
public:
    KeySet(Keys* k) : keys_( k )          { }

    // TODO: shared_ptr's or only 1 object (delete below)
    KeySet(const KeySet& k)               = delete;
    KeySet& operator=(const KeySet& k)    = delete;
    KeySet(KeySet&& k)                    = delete;


protected:
    // prims
    KeyKeyboardButton*        createKeyKeyboardButton(int code)                        { return keys_->createKeyKeyboardButton( code ); }
    KeyMouseButton*   createKeyMouseButton(int code)                   { return keys_->createKeyMouseButton( code ); }
    KeyMouseAxisX*    createKeyMouseAxisX()                            { return keys_->createKeyMouseAxisX(); }
    KeyMouseAxisY*    createKeyMouseAxisY()                            { return keys_->createKeyMouseAxisY(); }
    KeyMouseScroll*   createKeyMouseScrollX()                          { return keys_->createKeyMouseScrollY(); }
    KeyMouseScroll*   createKeyMouseScrollY()                          { return keys_->createKeyMouseScrollY(); }
    // cons
    KeyClicker*       createKeyClicker(Key* k)                         { return keys_->createKeyClicker( k ); }
    KeyAlpha*         createKeyAlpha(Key* k)                           { return keys_->createKeyAlpha( k ); }
    KeyPointer*       createKeyPointer(Key* x, Key* y, Key* l, Key* r) { return keys_->createKeyPointer( x, y, l, r ); }
    KeyPointer*       createKeyPointer()                               { return keys_->createKeyPointer(); }

    // create Key from definition
    Key* createKey(const YAML::Node& yaml)                            { return keys_->createKey( yaml ); }


    ////////////////////////////////////////////////////////////////////////////////
    //

private:
    Keys* keys_;

};


////////////////////////////////////////////////////////////////////////////////
//  

} // namespace keys

} // namespace batb


#endif
