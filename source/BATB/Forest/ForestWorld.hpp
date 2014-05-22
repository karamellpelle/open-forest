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
#ifndef BATB_FORESTWORLD_HPP
#define BATB_FORESTWORLD_HPP
#include "BATB/BATB_include.hpp"
#include "BATB/Forest/Camera.hpp"
#include "BATB/Forest/Terrain.hpp"
#include "BATB/Forest/Map.hpp"
#include "BATB/Forest/Course.hpp"
#include "BATB/Forest/Weather.hpp"

namespace BATB
{

class RunWorld;

class ForestWorld
{
public:
    typedef std::forward_list<Control>  ControlList;
    typedef std::forward_list<Map>      MapList;
    typedef std::forward_list<Runner>   RunnerList;

    ForestWorld(RunWorld* r) : run( r )    // worlds shall not be singletons!
    {
    }

    // viewing 
    Camera camera;

    // physical Terrain.
    // * ground
    // * trees
    // * vegetation
    // * other objects
    // ...
    Terrain terrain;

    // different views of Terrain. a Terrain can be viewed in different ways, 
    // for example old/new map, corridor orienteering, maps of different parts of terrain
    // TODO: polymorphism
    MapList maps;

    // runners 
    RunnerList runners;

    // current weather
    Weather weather;

    // events happened for this ForestWorld    
    ForestEventList events;

    // ForestNetwork (connected to RunNetwork, with a filter)

    // ForestWorld implies a RunWorld, the RunWorld containing 'this'.
    // it would be nice to let ForestWorld be totally independent of RunWorld,
    // so that ForestWorld works alone. however, ForestWorld needs access to
    // external resources. for exapmle Player's, containing information for each
    // Runner in ForestWorld. we let such external resources be part of RunWorld.
    // also, since there may (in theory) be more than one RunWorld, some of these
    // external resources could be different for two different RunWorld's.
    RunWorld* const run;
    
private:

};


}

#endif
