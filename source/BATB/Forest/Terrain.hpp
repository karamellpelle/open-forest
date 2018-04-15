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
#ifndef BATB_FOREST_TERRAIN_HPP
#define BATB_FOREST_TERRAIN_HPP
#include "OgreSceneManager.h"
#include "OgreViewport.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainPaging.h"
#include "OgrePageManager.h"
#include "OgrePagedWorld.h"
#include "OgreTerrainPagedWorldSection.h"
#include "include.hpp"

////////////////////////////////////////////////////////////////////////////////
// tmp!
//
//#define USE_SAMPLE_ENDLESSWORLD
#define USE_SAMPLE_TERRAIN

// importing demo/libs/ogre.cpp:
#ifdef USE_SAMPLE_ENDLESSWORLD
// max range for a int16
#define ENDLESS_TERRAIN_FILE_PREFIX String("EndlessWorldTerrain")
#define ENDLESS_TERRAIN_FILE_SUFFIX String("dat")
#define ENDLESS_PAGE_MIN_X (-0x7FFF)
#define ENDLESS_PAGE_MIN_Y (-0x7FFF)
#define ENDLESS_PAGE_MAX_X 0x7FFF
#define ENDLESS_PAGE_MAX_Y 0x7FFF
//#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_WORLD_SIZE 4000.0f
#define TERRAIN_SIZE 513
//#define HOLD_LOD_DISTANCE 3000.0
#define HOLD_LOD_DISTANCE 300.0
#endif

#ifdef USE_SAMPLE_TERRAIN
#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0
#define ENDLESS_TERRAIN_FILE_SUFFIX String("dat")
#define ENDLESS_PAGE_MIN_X (-0x7FFF)
#define ENDLESS_PAGE_MIN_Y (-0x7FFF)
#define ENDLESS_PAGE_MAX_X 0x7FFF
#define ENDLESS_PAGE_MAX_Y 0x7FFF
#define HOLD_LOD_DISTANCE 300.0

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
//#include "macUtils.h"
#endif

#define TERRAIN_FILE_PREFIX String("testTerrain")
#define TERRAIN_FILE_SUFFIX String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513
#endif



namespace batb
{


namespace forest
{


class Terrain
{
public:
    // [Tree] :         Terrain is populated with trees
    // other objects :  Terrain is populated with other objects

    Ogre::SceneManager*                 ogre_scenemgr = nullptr;
    Ogre::Viewport*                     ogre_viewport = nullptr;

    static Ogre::TerrainGlobalOptions*  ogre_terrain_globals; // only 1 per app; into BATB::Forest
    Ogre::TerrainGroup*                 ogre_terrain_group = nullptr;
    Ogre::TerrainPaging*                ogre_terrain_paging = nullptr;
    Ogre::PageManager*                  ogre_page_manager = nullptr;
    Ogre::PagedWorld*                   ogre_paged_world = nullptr;
    Ogre::TerrainPagedWorldSection*     ogre_paged_world_section = nullptr;

    ////////////////////////////////////////////////////////////////////////////////
    // methods
    //
    void load(const YAML::Node& );

    float_t incline(const glm::mat4& );

private:
};




} // namespace forest

} // namespace batb



#endif

