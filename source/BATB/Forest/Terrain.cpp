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
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Terrain.hpp"
#include "BATB/Forest/Terrain/TerrainPageProvider.hpp"

#include "OgreMaterialManager.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreEntity.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreViewport.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreResourceGroupManager.h"
#include "OgreLogManager.h"


#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTerrainPagedWorldSection.h"
#include "OgreTerrainAutoUpdateLod.h"
#include "OgreTerrainPaging.h"
#include "OgrePageManager.h"
#include "OgreRenderSystemCapabilities.h"

using namespace Ogre;



////////////////////////////////////////////////////////////////////////////////
// this file is based on the Sample_Terrain demo 
//


////////////////////////////////////////////////////////////////



namespace batb
{

namespace forest
{

// global
Ogre::TerrainGlobalOptions*  Terrain::ogre_terrain_globals = nullptr; 




////////////////////////////////////////////////////////////////////////////////
//

Terrain::Terrain(World& f) : forest( f )
{
    
}

Terrain::~Terrain()
{
    // Sample_Terrain::_shutdown()

    if ( ogre_terrain_paging )
    {
        delete ogre_terrain_paging;
        delete ogre_page_manager;
        delete page_provider;
    }
    else if ( ogre_terrain_group ) // FIXME: why else-if??
    {
        delete ogre_terrain_globals;
    }
}

////////////////////////////////////////////////////////////////////////////////
//

// compute incline at position and direction, [-1, 1]
// only z direction of 'aim' in xz plane is used, but maybe could we
// use the structure of mat4 in a natural way here?
float_t Terrain::incline(const glm::mat4& aim)
{
    // TODO: is there a function for this in OgreTerrainGroup/OgreTerrain?

    // measure step
    constexpr float_t x = 4.0; 

    const auto& p = aim[3];
    const auto& d = aim[2];

    auto u = glm::normalize( glm::vec2( d.x, d.z ) );
    auto p0 = glm::vec2( p.x, p.z );
    auto p1 = p0 + (float)( x ) * u;

    float_t z0 = ogre_terrain_group->getHeightAtWorldPosition( p0.x, 0.0, p0.y );
    float_t z1 = ogre_terrain_group->getHeightAtWorldPosition( p1.x, 0.0, p1.y );
    float_t z = z1 - z0;

    auto v = glm::normalize( glm::vec2( x, z ) );

    return v.y;
}



} // namespace forest

} // namespace batb

using namespace Ogre;



//#endif

