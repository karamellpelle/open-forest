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
#ifndef BATB_KEY_HPP
#define BATB_KEY_HPP
#include "BATB/Config.hpp"


namespace BATB
{

// base Key class
// abstracting a key as a continuous valueo [0, 1]
class Key
{
friend class Keys;
protected:
    Key()           { }
    virtual ~Key()  { }
public:
    virtual void clear() = 0;
    virtual void update(tick_t ) = 0;
    virtual float_t alpha() = 0; // FIXME: const??



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

private:

};


}

#endif
