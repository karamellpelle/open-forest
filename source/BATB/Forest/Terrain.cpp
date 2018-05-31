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
#define TERRAIN_FILE_PREFIX String("testTerrain")
#define TERRAIN_FILE_SUFFIX String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

bool terrains_imported = false;
Ogre::TerrainGroup* terrain_group = nullptr;
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
void initBlendMaps(Ogre::Terrain* terrain);
void defineTerrain(long x, long y, bool flat = false);

////////////////////////////////////////////////////////////////


// enable paging of terrain?
// FIXME: does not work (no terrain is created. paging not even enabled in Sample_Terrain)
//#define PAGING_ENABLE


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

void Terrain::load(const YAML::Node& yaml)
{
    ////////////////////////////////////////////////////////////////////////////////
    // Sample_Terrain::setupContent()

    // create the only TerrainGlobalOptions, if not present
    // see https://ogrecave.github.io/ogre/api/1.11/class_ogre_1_1_terrain_global_options.html
    if ( ogre_terrain_globals == nullptr )
    {
        ogre_terrain_globals = new TerrainGlobalOptions();
        // TODO: setup ?
    }

    MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
    MaterialManager::getSingleton().setDefaultAnisotropy(7);


    ogre_terrain_group = OGRE_NEW TerrainGroup(ogre_scenemanager, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    ogre_terrain_group->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
    terrain_group = ogre_terrain_group; // since helper functions below are not class members


    ////////////////////////////////////////////////////////////////
    // Sample_Terrain::configureTerrainDefaults(Light* l)

    // Configure global
    ogre_terrain_globals->setMaxPixelError( 8 );
    // testing composite map
    ogre_terrain_globals->setCompositeMapDistance( 3000 );
    //mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
    //mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
    //mTerrainGlobals->setLightMapSize(256);

    // Disable the lightmap for OpenGL ES 2.0. The minimum number of samplers allowed is 8(as opposed to 16 on desktop).
    // Otherwise we will run over the limit by just one. The minimum was raised to 16 in GL ES 3.0.
    if (Ogre::Root::getSingletonPtr()->getRenderSystem()->getCapabilities()->getNumTextureUnits() < 9)
    {
        TerrainMaterialGeneratorA::SM2Profile* matProfile =
            static_cast<TerrainMaterialGeneratorA::SM2Profile*>( ogre_terrain_globals->getDefaultMaterialGenerator()->getActiveProfile());
        matProfile->setLightmapEnabled(false);
    }

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& terrain_import = ogre_terrain_group->getDefaultImportSettings();
    terrain_import.terrainSize = TERRAIN_SIZE;
    terrain_import.worldSize = TERRAIN_WORLD_SIZE;
    terrain_import.inputScale = 600;
    terrain_import.minBatchSize = 33;
    terrain_import.maxBatchSize = 65;
    // textures
    // see 'initBlendMaps below 
    terrain_import.layerList.resize(3);
    terrain_import.layerList[0].worldSize = 400;
    terrain_import.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    terrain_import.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    terrain_import.layerList[1].worldSize = 30;
    terrain_import.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    terrain_import.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    terrain_import.layerList[2].worldSize = 200;
    terrain_import.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    terrain_import.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////
    // Sample_Terrain::setupContent()
#ifdef PAGING_ENABLE
    // Paging setup
    ogre_page_manager = OGRE_NEW PageManager();
    // Since we're not loading any pages from .page files, we need a way just 
    // to say we've loaded them without them actually being loaded
    page_provider = new TerrainPageProvider(); // custom class
    ogre_page_manager->setPageProvider( page_provider );
    ogre_page_manager->addCamera( ogre_scenemanager->getCamera(  "ogre_camera" ) );
    ogre_page_manager->setDebugDisplayLevel( 0 );
    ogre_terrain_paging = OGRE_NEW TerrainPaging( ogre_page_manager );
    ogre_paged_world = ogre_page_manager->createWorld();
    ogre_paged_world_section = ogre_terrain_paging->createWorldSection(ogre_paged_world, ogre_terrain_group, 2000, 3000,
                                           TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y,
                                           TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y);

#else
    bool blankTerrain = false;
    for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
        for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
            defineTerrain(x, y, blankTerrain);
    // sync load since we want everything in place when we start
    ogre_terrain_group->loadAllTerrains(true);
#endif

    if (terrains_imported)
    {
        TerrainGroup::TerrainIterator ti = ogre_terrain_group->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }
    ogre_terrain_group->updateDerivedData();

    ogre_terrain_group->freeTemporaryResources();

}

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


    void defineTerrain(long x, long y, bool flat )
    {
        // if a file is available, use it
        // if not, generate file from import

        // Usually in a real project you'll know whether the compact terrain data is
        // available or not; I'm doing it this way to save distribution size

        if (flat)
        {
            terrain_group->defineTerrain(x, y, 0.0f);
        }
        else
        {
            String filename = terrain_group->generateFilename(x, y);
            if (ResourceGroupManager::getSingleton().resourceExists(terrain_group->getResourceGroup(), filename))
            {
                terrain_group->defineTerrain(x, y);
            }
            else
            {
                Image img;
                getTerrainImage(x % 2 != 0, y % 2 != 0, img);
                terrain_group->defineTerrain(x, y, &img);
                terrains_imported = true;
            }
        }
    }

    void getTerrainImage(bool flipX, bool flipY, Image& img)
    {
        img.load("terrain.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        if (flipX)
            img.flipAroundY();
        if (flipY)
            img.flipAroundX();
    }

    void initBlendMaps(Terrain* terrain)
    {
        TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
        TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
        Real minHeight0 = 70;
        Real fadeDist0 = 40;
        Real minHeight1 = 70;
        Real fadeDist1 = 15;
        float* pBlend1 = blendMap1->getBlendPointer();
        for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
        {
            for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
            {
                Real tx, ty;

                blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
                Real height = terrain->getHeightAtTerrainPosition(tx, ty);
                Real val = (height - minHeight0) / fadeDist0;
                Math::Clamp(val, (Real)0, (Real)1);

                val = (height - minHeight1) / fadeDist1;
                val = Math::Clamp(val, (Real)0, (Real)1);
                *pBlend1++ = val;


            }
        }
        blendMap0->dirty();
        blendMap1->dirty();
        //blendMap0->loadImage("blendmap1.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        blendMap0->update();
        blendMap1->update();

        // set up a colour map
        /*
          if (!terrain->getGlobalColourMapEnabled())
          {
          terrain->setGlobalColourMapEnabled(true);
          Image colourMap;
          colourMap.load("testcolourmap.jpg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
          terrain->getGlobalColourMap()->loadImage(colourMap);
          }
        */
    }

//#endif

