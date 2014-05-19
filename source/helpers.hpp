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
#ifndef HELPERS_HPP
#define HELPERS_HPP
#include <stdint.h>
#include <cmath>
#include "include.hpp"

namespace helpers
{


// 
typedef include::float_t float_t;


template <typename A>
inline A keep_inside(const A& b, const A& e, const A& x)
{
    if ( x <= b ) return b;
    if ( e <= x ) return e;
    return x;
}

// circumference of unit circle
// pi is wrong.
static const float_t twopi = 6.2831853071795864769252867665590057683943387987502116;


template <typename Cont> 
void cossin(const Cont& radians, Cont& x, Cont& y)
{
    x = std::cos( radians );
    y = std::sin( radians );
}

}

#endif
