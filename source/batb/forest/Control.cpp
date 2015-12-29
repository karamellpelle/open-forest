//    open-forest: an orientering game.
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
#include "OgreEntity.h"
#include "OgreTerrainGroup.h"
#include "OgreSceneNode.h"
#include "batb.hpp"
#include "batb/forest/Control.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"


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

        forest.ogre_scenemgr->destroyEntity( ogre_entity );
        forest.ogre_scenemgr->destroySceneNode( node );


    }
    ogre_entity = nullptr;

}
void Control::reset(const ControlDefinition& def)
{
    ////////////////////////////////////////////////////////////////////////////////
    // set correct height from terrain
    float_t x = def.x;
    float_t z = def.z;
    float_t y = forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( 
                Ogre::Vector3( x, 0.0, z ) );

    aim[3].x = x;
    aim[3].y = y;
    aim[3].z = z;
    aim[3].w = 1.0;

    // ensure previous data is cleared
    reset();

    ////////////////////////////////////////////////////////////////////////////////
    // create output
    //
    std::ostringstream name;
    name << "Control_" << def.code << "(" << this << ")";

    ogre_entity = forest.ogre_scenemgr->createEntity( name.str(), "control.mesh" );
    auto* node = forest.ogre_scenemgr->getRootSceneNode()->createChildSceneNode();
    node->scale( 16, 16, 16 ); // FIXME
    node->setPosition( aim[3].x, aim[3].y, aim[3].z );
    node->attachObject( ogre_entity );

    // set definition
    definition = def;
}
  
void Control::punch(Runner* runner)
{
    // (no check for distance)
      
    ++stats_punches;

}

} // namespace forest

} // namespace batb



