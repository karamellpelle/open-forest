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




namespace batb
{


namespace forest
{

class World;

// this custom class loads our terrain dynamically
class TerrainPageProvider;


class Terrain
{
public:
    Terrain(World& );
    ~Terrain();

    void load(const YAML::Node& );

    float_t incline(const glm::mat4& );

    // [Tree] :         Terrain is populated with trees
    // other objects :  Terrain is populated with other objects


    ////////////////////////////////////////////////////////////////////////////////
    World& forest;


    Ogre::SceneManager*                 ogre_scenemanager = nullptr;
    Ogre::Viewport*                     ogre_viewport = nullptr;

    Ogre::TerrainGroup*                 ogre_terrain_group = nullptr;
    Ogre::TerrainPaging*                ogre_terrain_paging = nullptr;
    Ogre::PageManager*                  ogre_page_manager = nullptr;
    Ogre::PagedWorld*                   ogre_paged_world = nullptr;
    Ogre::TerrainPagedWorldSection*     ogre_paged_world_section = nullptr;
    TerrainPageProvider*                page_provider = nullptr;

    static Ogre::TerrainGlobalOptions*  ogre_terrain_globals; 

    
private:

};




} // namespace forest

} // namespace batb



#endif

