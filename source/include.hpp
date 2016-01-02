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
#ifndef INCLUDE_HPP
#define INCLUDE_HPP
#include <cmath>
#include <cstdint> 
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include "definitions.hpp"

// main include file to be included through all sources...



namespace include
{

// our type for the sizes of finite sets.
// we requirere a word size of minimum 32 bits.
// size_t is for memory, https://en.wikipedia.org/wiki/Stdint.h#stdint.h
typedef std::uint_fast32_t uint;

// our floating point type.
// FIXME: fastest/best for platform.
typedef double float_t;


// circumference of unit circle
// pi is wrong.
constexpr float_t twopi = 6.2831853071795864769252867665590057683943387987502116;


// interpolating from alpha
template <typename Linear, typename Scalar>
Linear smooth(Linear a, Linear b, Scalar alpha)
{
    return ((Scalar)( 1.0 ) - alpha) * a + alpha * b;
}


// keep value inside span
template <typename A>
inline A keep_inside(const A& b, const A& e, const A& x)
{
    if ( x <= b ) return b;
    if ( e <= x ) return e;
    return x;
}


// compute unit x,y values, from radian angle
template <typename Angle, typename Real> 
void cossin(const Angle& radians, Real& x, Real& y)
{
    x = std::cos( radians );
    y = std::sin( radians );
}

}

#endif
