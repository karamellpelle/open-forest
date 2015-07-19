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
#ifndef BATB_KEYS_KEYSET_HPP
#define BATB_KEYS_KEYSET_HPP
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
    KeySet(Keys& k) : keys_( k )          { }

    // TODO: shared_ptr's or only 1 object (delete below)
    KeySet(const KeySet& k)               = delete;
    KeySet& operator=(const KeySet& k)    = delete;
    KeySet(KeySet&& k)                    = delete;


protected:
    // prims
    KeyButton*      createKeyButton(int code)                         { return keys_.createKeyButton( code ); }
    KeyMouseButton* createKeyMouseButton(int code)                    { return keys_.createKeyMouseButton( code ); }
    KeyMouseAxisX*  createKeyMouseAxisX()                             { return keys_.createKeyMouseAxisX(); }
    KeyMouseAxisY*  createKeyMouseAxisY()                             { return keys_.createKeyMouseAxisY(); }
    // cons
    KeyClicker*     createKeyClicker(Key* k)                          { return keys_.createKeyClicker( k ); }
    KeyAlpha*       createKeyAlpha(Key* k)                            { return keys_.createKeyAlpha( k ); }
    KeyPointer*     createKeyPointer(Key* x, Key* y, Key* l, Key* r)  { return keys_.createKeyPointer( x, y, l, r ); }
    KeyPointer*     createKeyPointer()                                { return keys_.createKeyPointer(); }

    // create Key from definition
    Key* createKey(const YAML::Node& yaml)                            { return keys_.createKey( yaml ); }


    ////////////////////////////////////////////////////////////////////////////////
    //

private:
    Keys& keys_;

};


////////////////////////////////////////////////////////////////////////////////
//  

} // namespace keys

} // namespace batb


#endif
