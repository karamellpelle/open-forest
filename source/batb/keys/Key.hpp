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
    Key(const std::type_index& ix, Keys& k) : keys_( k ), type_( ix ) { }
    virtual ~Key()                                                    { }

    // TODO: if using shared_ptr's, set these to default:
    Key(const Key& k)             = delete;
    Key& operator=(const Key& k)  = delete;
    Key(Key&& k)                  = delete;

    // this implements the empty key
    virtual void reset() { }
    virtual void step(tick_t ) { }
    virtual float_t alpha() { return 0.0; }

    //////////////////////////////////////////////////////////////////
    //      helpers
    //
    float_t alpha(float_t b, float_t e)
    {
        float_t a  = alpha();
        return (1.0 - a) * b + a * e;
    }
    bool zero()
    {
        return alpha() == 0.0;
    }
    bool one()
    {
        return alpha() == 1.0;
    }
    bool is_down()
    {
        return 0.5 < alpha(); // NOTE: strict larger than
    }


protected:

    Keys& keys_;
    std::type_index type_;
};

} // namespace keys

} // namespace batb

#endif

