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
#ifndef BATB_KEYS_KEY_HPP
#define BATB_KEYS_KEY_HPP
#include <typeinfo>
#include <typeindex>
#include "batb/batb_include.hpp"


namespace batb
{


namespace keys
{

class Keys;

// base Key class
// abstracting a key as a continuous value in \RR
class Key
{
friend class Keys;

public:
    Key(Keys& k) : keys_( k )       { /*++refs_;*/ }
    virtual ~Key()                  { }

    // TODO: if later using shared_ptr's, set these to default:
    Key(const Key& k)             = delete;
    Key& operator=(const Key& k)  = delete;
    Key(Key&& k)                  = delete;

    // can this key be disabled by Keys? (default is true)
    virtual void canDisable(bool b) { can_disable = b; }
    // this implements the empty key
    virtual void reset()            { }
    virtual void step(tick_t )      { }
    virtual float_t alpha()         { return 0.0; }

    //////////////////////////////////////////////////////////////////
    //      helpers
    //
    float_t alpha(float_t b, float_t e) { float_t a  = alpha(); return (1.0 - a) * b + a * e; }
    bool zero()                         { return alpha() == 0.0; }
    bool one()                          { return alpha() == 1.0; }
    bool is_down()                      { return 0.5 < alpha();  } // NOTE: strictly larger


protected:
    Keys& keys_;
    //uint refs_ = 0;
    bool can_disable = true;  // all Key's should in general be disabled if requested by Keys
};

} // namespace keys

} // namespace batb

#endif

