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
#include "BATB/Run.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Runner.hpp"
#include "BATB/AL.hpp"
#include "BATB/GL.hpp"
#include "BATB/OGRE.hpp"

#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreTerrainGroup.h"
#include "OgreShaderGenerator.h"
#include "OgreTerrainMaterialGeneratorA.h"

#include "OgrePageManager.h"
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTerrainPaging.h"
#include "OgreShadowCameraSetupPSSM.h"
#include "OgreMaterialManager.h"
#include "OgreTechnique.h"


namespace batb
{

namespace forest
{


World::World(run::World* r) : terrain( *this ), weather( this ),  run( r )
{ 

}

// update states
void World::update()
{
    // compute (update) all DTMovable's in World
    // for now, only Runner is movable
    for (auto& runner : runners )
    {
        runner.move.compute();
    }

    // update Runners
    for (auto& runner : runners )
    {
        runner.update();
    }

    // TODO: update other parts

}

Control* World::addControl(const ControlDefinition& def)
{

    // create Control in rigid memory (pointers valid for std::forward_list!)
    controls.push_front( Control( *this ) );
    auto* control = &controls.front();

    // place it in terrain
    control->reset( def );
   
    return control;
}

void World::removeControl(Control* control)
{
    // release data
    control->reset();

    // this invalidates 'control' !!
    auto pred = [=](const Control& c) { return &c == control; };
    controls.remove_if( pred ); // goes through whole list even if 'control' found

}

Runner* World::addRunner(Player* player)
{

    // create Runner in rigid memory (pointers valid for std::forward_list!)
    runners.push_front( Runner( *this, player ) );
    auto* runner = &runners.front();
    
    runner->reset( YAML::Node() ); // TODO: definition

    return runner;
}




////////////////////////////////////////////////////////////////////////////////
// WorldLoader
//





} // namespace forest

} // namespace batb

