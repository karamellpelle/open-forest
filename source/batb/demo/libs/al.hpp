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
#ifndef BATB_DEMO_LIBS_AL_HPP
#define BATB_DEMO_LIBS_AL_HPP
#include "batb/batb_include.hpp"

namespace batb
{

class BATB;

namespace run
{
class World;
}


namespace demo
{

namespace al
{


void demo_begin(BATB& );
void demo_end(BATB& );

void demo_play(BATB& , const std::string& );

void demo_iterate(BATB& , run::World& );



} // namespace al

} // namespace demo

} // namespace batb


#endif


