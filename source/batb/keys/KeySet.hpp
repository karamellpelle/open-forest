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

// create Key's
// 
// should typically be subclassed into a class having members
// of each Key necessary. this subclass should typically have
// a method 'load(const std::string& filepath)' to load
// a definition from a config (YAML) file.
class KeySet
{
public:
    KeySet()                              { }
    ~KeySet()                             { clear(); }
    KeySet(const KeySet& k)               = delete;
    KeySet& operator=(const KeySet& k)    = delete;
    KeySet(KeySet&& k)                    = delete;


    // prims
    KeyButton*      createKeyButton(KeyButton::Code code)             { return push( new KeyButton( code ) ); }
    KeyMouseButton* createKeyMouseButton(KeyMouseButton::Code code)   { return push( new KeyMouseButton( code ) ); }
    KeyMouseAxisX*  createKeyMouseAxisX()                             { return push( new KeyMouseAxisX() ); }
    KeyMouseAxisY*  createKeyMouseAxisY()                             { return push( new KeyMouseAxisY() ); }
    // cons
    KeyClicker*     createKeyClicker(Key* k)                          { return push( new KeyClicker( k ) ); }
    KeyAlpha*       createKeyAlpha(Key* k)                            { return push( new KeyAlpha( k ) ); }
    KeyPointer*     createKeyPointer(Key* x, Key* y, Key* l, Key* r)  { return push( new KeyPointer( x, y, l, r ) ); }

    // create Key from definition
    Key* createKey(/* std::string&*/ /*XML def*/ );


    ////////////////////////////////////////////////////////////////////////////////
    //

    // invalidate and delete all keys
    // NOTE: this is very different from Key::clear()!!
    void clear();

    // Keys::clear() for all keys:
    // FIXME: rename Key::clear() -> Key::reset()
    void reset();

    // update all keys, to be done at end of each frame:
    // FIXME: rename Key::update(tick_t) -> Key::step(tick_t)
    void step(tick_t t);
   

private:
    typedef std::vector<Key*> Container;
    Container keys_;

    template <typename KeyPtr>
    KeyPtr push(KeyPtr k)
    {
        keys_.push_back( k );
        return k;
    }

};


////////////////////////////////////////////////////////////////////////////////
//  

} // namespace keys

} // namespace batb


#endif
