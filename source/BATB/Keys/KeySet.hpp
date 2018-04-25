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
// a method 'load(const std::string& filepath)' to load
// a definition from a config (YAML) file.
class KeySet
{
public:
    KeySet(BATB* b) : batb( b )          { }

    // TODO: shared_ptr<Key> or only 1 object (delete below)
    KeySet(const KeySet& k)               = delete;
    KeySet& operator=(const KeySet& k)    = delete;
    KeySet(KeySet&& k)                    = delete;


protected:
    // prims
    KeyKeyboardButton* createKeyKeyboardButton(code::KeyboardButton code) { return batb->keys->createKeyKeyboardButton( code ); }
    KeyMouseButton*    createKeyMouseButton(code::MouseButton code)       { return batb->keys->createKeyMouseButton( code );    }
    KeyMouseAxis*      createKeyMouseAxis(code::MouseAxis code)           { return batb->keys->createKeyMouseAxis( code );      }
    KeyMouseScroll*    createKeyMouseScroll(code::MouseScroll code)       { return batb->keys->createKeyMouseScroll( code );    }
    // cons
    KeyClicker*        createKeyClicker(Key* k)                           { return batb->keys->createKeyClicker( k );           }
    KeyAlpha*          createKeyAlpha(Key* k)                             { return batb->keys->createKeyAlpha( k );             }
    KeyPointer*        createKeyPointer(Key* x, Key* y, Key* l, Key* r)   { return batb->keys->createKeyPointer( x, y, l, r );  }
    KeyPointer*        createKeyPointer()                                 { return batb->keys->createKeyPointer();              }

    // create Key from definition
    Key*               createKey(const YAML::Node& yaml)                  { return batb->keys->createKey( yaml );               }
    // prims
    KeyKeyboardButton* createKeyKeyboardButton(const YAML::Node& y)       { return batb->keys->createKeyKeyboardButton( y );    }
    KeyMouseButton*    createKeyMouseButton(const YAML::Node& y)          { return batb->keys->createKeyMouseButton( y );       }
    KeyMouseAxis*      createKeyMouseAxis(const YAML::Node& y)            { return batb->keys->createKeyMouseAxis( y );         }
    KeyMouseScroll*    createKeyMouseScroll(const YAML::Node& y)          { return batb->keys->createKeyMouseScroll( y );       }
    // cons
    KeyClicker*        createKeyClicker(const YAML::Node& y)              { return batb->keys->createKeyClicker( y );           }
    KeyAlpha*          createKeyAlpha(const YAML::Node& y)                { return batb->keys->createKeyAlpha( y );             }
    KeyPointer*        createKeyPointer(const YAML::Node& y)              { return batb->keys->createKeyPointer( y );           }

    // helper: if nullptr, create the default of same type, if possible
    template <typename K, typename... Args>
    K*                 safeKey(K* , Args... );

    ////////////////////////////////////////////////////////////////////////////////
    //

    BATB* batb = nullptr;

};

template <typename K, typename... Args>
inline K* KeySet::safeKey(K* key, Args... args)
{
    if ( key ) return key;

    batb->log << "! WARNING: using empty Key instead";

    // create 
    return batb->keys->create<K>( args... );

}


////////////////////////////////////////////////////////////////////////////////
//  

} // namespace keys

} // namespace batb


#endif
