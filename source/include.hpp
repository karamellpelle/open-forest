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
#ifndef INCLUDE_HPP
#define INCLUDE_HPP
//#include <cstdint> // <- c++11
#include <stdint.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include "helpers.hpp"

// main include file to be included through all sources...




// requirering a word size of minimum 32 bits
// size_t is for memory...
// https://en.wikipedia.org/wiki/Stdint.h#stdint.h

//typedef std::uint_fast32_t uint; // <- c++11
typedef uint_fast32_t uint;

// we need a floating type. FIXME: fastest for platform
typedef double float_t;


#endif
