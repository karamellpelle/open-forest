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
#ifndef BATB_FOREST_WORLD_HPP
#define BATB_FOREST_WORLD_HPP
#include "BATB/Forest.hpp"
#include "BATB/Forest/Camera.hpp"
#include "BATB/Forest/Terrain.hpp"
#include "BATB/Forest/Map.hpp"
#include "BATB/Forest/Control.hpp"
#include "BATB/Forest/Runner.hpp"
#include "BATB/Forest/Weather.hpp"
#include "BATB/Event.hpp"
#include "BATB.hpp"
#include "AL/alure2.h"



namespace Ogre
{
    class SceneManager;
    class Viewport;
}


namespace batb
{

class BATB;

namespace run { class World; }


namespace forest
{


class World
{

public:
    typedef std::forward_list<Control>  ControlList;
    typedef std::forward_list<Map>      MapList;
    typedef std::forward_list<Runner>   RunnerList;

    World(run::World* r); 

    ////////////////////////////////////////////////////////////////////////////////
    //

    // Control
    Control* addControl(const ControlDefinition& );
    void removeControl(Control* );

    // Runner
    Runner* addRunner(Player* = nullptr);
    

    // viewing 
    Camera camera;

    // physical Terrain.
    // * ground
    // * trees
    // * vegetation
    // * other objects
    // ...
    Terrain terrain;

    ControlList controls;

    // different views of Terrain. a Terrain can be viewed in different ways, 
    // for example old/new map, corridor orienteering, maps of different parts of terrain
    MapList maps;

    // Runners
    RunnerList runners;

    // current weather
    Weather weather;

    // events happened for this ForestWorld    
    EventList events;

    // ForestNetwork (connected to RunNetwork, with a filter)

    // forest::World implies a run::World, the run::World containing 'this'.
    // it would be nice to let forest::World be totally independent of run::World,
    // so that forest::World works alone. however, forest::World needs access to
    // external resources. for exapmle Player's, containing information for each
    // Runner in forest::World. we let such external resources be part of run::World.
    // also, since there may be more than one forest::World, some of these
    // external resources could be different for two different forest::World's.
    // also, 'run' is a reference, hence there is one and only one run::World asso-
    // ciated with each forest::World
    // TODO: remove this
    run::World* run = nullptr;

    
    // tick of world
    tick_t tick = 0.0;


    // number of frames iterated (by IterationForest...)
    uint frames = 0;


    Ogre::SceneManager* ogre_scenemanager = nullptr;
    Ogre::Viewport*     ogre_viewport = nullptr;


    // sound
    alure::Listener alure_listener;
private:
    
    
};


// setup and destroy a World
class WorldLoader
{
public:
    WorldLoader(BATB* );
    //WorldLoader(BATB* , run::Work& );

    void load(World& , const YAML::Node& );
    void unload(World& );

private:
    void ogreShadows(World& );

    BATB* batb = nullptr;


};


} // namespace forest

} // namespace batb




#endif
