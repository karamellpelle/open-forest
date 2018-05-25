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
#include "BATB/Player.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Forest/events.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Control.hpp"

#include "OgreEntity.h"
#include "OgreTerrainGroup.h"
#include "OgreSceneNode.h"


namespace batb
{

namespace forest
{

void Control::reset()
{
    if ( ogre_entity )
    {
        auto* node = ogre_entity->getParentSceneNode();
        ogre_entity->detachFromParent(); // remove parent Node (SceneNode)

        forest.ogre_scenemanager->destroyEntity( ogre_entity );
        forest.ogre_scenemanager->destroySceneNode( node );


    }
    ogre_entity = nullptr;

}
void Control::reset(const ControlDefinition& def)
{
    // ensure previous data is cleared
    reset();

    // set correct height from terrain
    aim.pos.x = def.x;
    aim.pos.z = def.z;
    aim.pos.y = forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( 
                Ogre::Vector3( aim.pos.x, 0.0, aim.pos.z ) );
    aim.pos.w = 1.0;


    ////////////////////////////////////////////////////////////////////////////////
    // create output
    //
    std::ostringstream os( "Control_" );
    os << def.code << "(" << this << ")";

    ogre_entity = forest.ogre_scenemanager->createEntity( os.str(), "control.mesh" );
    auto* node = forest.ogre_scenemanager->getRootSceneNode()->createChildSceneNode();
    node->scale( 16, 16, 16 ); // FIXME
    node->setPosition( aim.pos.x, aim.pos.y, aim.pos.z );
    node->attachObject( ogre_entity );

    // set definition
    definition = def;
}
  
void Control::punch(Runner* runner)
{
    // (no check for distance)
      
    ++stats_punches;

    // add event
    forest.events.push( event::ControlPunch( forest.tick, runner, this ) );

    // tmp:
    std::cout
    //batb.run.console 
                      << (runner->player == nullptr ? "a runner" : runner->player->name)
                      << " punched control " << definition.code
                      << std::endl
                      ;
}

} // namespace forest

} // namespace batb



