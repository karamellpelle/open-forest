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
#ifndef BATB_RUN_WORLD_HPP
#define BATB_RUN_WORLD_HPP
#include "batb/batb_include.hpp"
#include "batb/Scene.hpp"
//#include "BATB/Run/RunEvent.hpp"
//#include "BATB/Run/Player.hpp"
//#include "BATB/Forest.hpp"
//#include "BATB/Race.hpp"


namespace batb
{


namespace run
{

class World
{
public:
    World();

    // rendering target:
    Scene scene;

    //RaceWorld race;
    //IterationStackRaceWorld race_stack;

    // RunNetwork network;
    // [RunEvent] events;

    // tick of world
    tick_t tick;

    // number of frames iterated (by IterationRun...)
    uint frames_count = 0;
    
};


} // namespace run

} // namespace batb

#endif
