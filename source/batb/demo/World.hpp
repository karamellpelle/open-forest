//    open-demo: an orientering game.
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
#ifndef BATB_DEMO_DEMO_WORLD_HPP
#define BATB_DEMO_DEMO_WORLD_HPP
#include "batb/batb_include.hpp"
#include "batb/demo/Camera.hpp"
#include "batb/demo/Terrain.hpp"
#include "batb/demo/Map.hpp"
#include "batb/demo/Course.hpp"
#include "batb/demo/Control.hpp"
#include "batb/demo/Runner.hpp"
#include "batb/demo/Weather.hpp"
#include "batb/event.hpp"


namespace batb
{

namespace run
{
class World;
}

namespace demo
{


class World
{

public:
    World(run::World& r) : run( r ) { }
    // demo::World implies a run::World, the run::World containing 'this'.
    // it would be nice to let demo::World be totally independent of run::World,
    // so that demo::World works alone. however, demo::World needs access to
    // external resources. for exapmle Player's, containing information for each
    // Runner in demo::World. we let such external resources be part of run::World.
    // also, since there may be more than one demo::World, some of these
    // external resources could be different for two different demo::World's.
    // also, 'run' is a reference, hence there is one and only one run::World asso-
    // ciated with each demo::World
    run::World& run;

    
    // tick of world
    tick_t tick = 0.0;

private:
    
};


} // namespace demo

} // namespace batb




#endif
