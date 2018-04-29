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

//
// hold Key's
// 
// should typically be subclassed into a class having members
// of each Key necessary. this subclass should typically have
// a method 'load(const YAML::Node& )' to load
// a definition from a config YAML::Node
class KeySet
{
public:
    KeySet(BATB* b) : batb( b )          { }

    // TODO: shared_ptr<Key> or only 1 object (delete below)
    KeySet(const KeySet& k)               = delete;
    KeySet& operator=(const KeySet& k)    = delete;
    KeySet(KeySet&& k)                    = delete;


protected:
    template <typename K, typename... Args>
    K*                 create(Args... args)                               { return batb->keys->create<K>( args... );               }
    template <typename K, typename... Args>
    K*                 createSafe(Args... args)                           { return batb->keys->createSafe<K>( args... );           }
    ////////////////////////////////////////////////////////////////////////////////

    // create Key from definition
    Key*               createKey(const YAML::Node& yaml)                     { return batb->keys->createKey( yaml );              }
    Key*               createSafeKey(const YAML::Node& yaml)                 { return batb->keys->createSafeKey( yaml );          }


    BATB* batb = nullptr;

};

////////////////////////////////////////////////////////////////////////////////
//  

} // namespace keys

} // namespace batb


#endif
