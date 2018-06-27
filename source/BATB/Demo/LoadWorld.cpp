//    open-demo: an orientering game.
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
#include "BATB/Demo/World.hpp"
#include "BATB/Demo/LoadWorld.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/LoadWorld.hpp"
#include "BATB/Run/World.hpp"



namespace batb
{
namespace demo
{

LoadWorld::LoadWorld(BATB* b)
{
    batb = b;
}



void LoadWorld::load(const YAML::Node& yaml, World* demo)
{
    auto* forest = demo->forest.get();

    // load subworld first
    forest::LoadWorld( batb ).load( yaml[ "forest" ], forest );

    // make sure stuff are connected

    // connect WorldDrawer 
    demo->forest_drawer.init( forest );
    // connect MapDrawer
    demo->map_drawer.init( demo );
    // connect Course
    demo->course.init( forest );

}


void LoadWorld::unload(World& demo)
{
    batb->log << "FIXME: unload demo::World!" << std::endl;
}





} // namespace demo

} // namespace batb

