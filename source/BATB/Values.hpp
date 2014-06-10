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
#ifndef BATB_VALUES_HPP
#define BATB_VALUES_HPP
#include "tinyxml2.h"
#include "BATB/BATB_include.hpp"


// this file contains values to be used throughout the BATB-program. some are constants, but other are not. the 
// non-constants may defined runtime from for example a (the) configuration file. should these values be
// allowed to change after they are initialized? ideally not. but maybe from a program change from settings,
// for example. 
namespace BATB
{

class Values
{
public:
    static void create(tinyxml2::XMLElement* );
    static void destroy();
};

}


namespace BATB
{

extern float_t valueProj3DNear;
extern float_t valueProj3DFar;
extern float_t valueProj3DFOVY;

}

#endif
