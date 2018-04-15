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
#ifndef INCLUDE_HPP
#define INCLUDE_HPP

// STL
#include <cmath>
#include <cstdint> 
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include "definitions.hpp"
#include <vector>
#include <cstdio>
#include <list>
#include <forward_list>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <random>
#include <mutex>
#include <array>
#include <limits>

#ifdef __unix__
#include <unistd.h>
#include <pwd.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// debug

// macro for function name, depends on compiler
// FIXME: ifdef COMPILER_X ...
#define DEBUG_FUNCTION_NAME __PRETTY_FUNCTION__

#include "debug/gl.hpp"

////////////////////////////////////////////////////////////////////////////////
// GLM
// see: http://www.glm.g-truc.net/glm.pdf
#define GLM_SWIZZLE           // implement swizzles (increses compilation time)
//#define GLM_PRECISION_XXX // define precisions
//#define GLM_MESSAGES          // let glm print system messages
#define GLM_FORCE_RADIANS       // let glm angles be in radians
//#define GLM_FORCE_INLINE      // let glm be inlined
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/type_ptr.hpp"

//
////////////////////////////////////////////////////////////////////////////////
//  tinyxml2
//#include "tinyxml2.h"

////////////////////////////////////////////////////////////////////////////////
//  file
#include "file.hpp"

////////////////////////////////////////////////////////////////////////////////
//  yaml
#include "yaml-cpp/yaml.h"


// our type for the sizes of finite sets.
// we requirere a word size of minimum 32 bits.
// size_t is for memory, https://en.wikipedia.org/wiki/Stdint.h#stdint.h
typedef std::uint_fast32_t uint;

// our floating point type.
// FIXME: fastest/best for platform.

// http://en.cppreference.com/w/c/numeric/math/float_t
typedef std::float_t float_t;


// circumference of unit circle
// pi is wrong.
constexpr float_t twopi = 6.2831853071795864769252867665590057683943387987502116;


// interpolating from alpha
template <typename Linear, typename Scalar>
constexpr Linear smooth(const Linear& a, const Linear& b, const Scalar& alpha)
{
    return ( (Scalar)( 1.0 ) - alpha ) * a + alpha * b;
}

// this one is part of <algorithm> in C++17
template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    if ( v < lo ) return lo;
    if ( hi < v ) return hi;
    return v;
}



// compute unit x,y values, from radian angle
template <typename Angle, typename Real> 
void cossin(const Angle& radians, Real& x, Real& y)
{
    x = std::cos( radians );
    y = std::sin( radians );
}

typedef double tick_t;

#endif
