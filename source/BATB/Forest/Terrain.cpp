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
#include "BATB/Demo/libs/ogre/PerlinNoiseTerrainGenerator.h"

#include "OgreMaterialManager.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreEntity.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreViewport.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
//#include "OgreWindowEventUtilities.h"
#include "OgreResourceGroupManager.h"

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

static PerlinNoiseTerrainGenerator* perlin_noise;
static TerrainGroup* terrain_group = nullptr;
bool terrains_imported = false;
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
void initBlendMaps(Ogre::Terrain* terrain);
void defineTerrain(long x, long y, bool flat = false);

namespace batb
{

namespace forest
{

// global!
Ogre::TerrainGlobalOptions*  Terrain::ogre_terrain_globals = nullptr; 



////////////////////////////////////////////////////////////////////////////////
class DummyPageProvider : public Ogre::PageProvider
{
public:
    bool prepareProceduralPage(Page* page, PagedWorldSection* section)    { return true; }
    bool loadProceduralPage(Page* page, PagedWorldSection* section)       { return true; }
    bool unloadProceduralPage(Page* page, PagedWorldSection* section)     { return true; }
    bool unprepareProceduralPage(Page* page, PagedWorldSection* section)  { return true; }

}
dummy_page_provider;



void Terrain::load(const YAML::Node& yaml)
{
#ifdef USE_SAMPLE_TERRAIN
    if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists("Terrain"))
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Terrain");
#endif

    // create the only TerrainGlobalOptions, if not present
    // Terrain::setResourceGroup overrides this (??)
    if ( ogre_terrain_globals == nullptr )
    {
        ogre_terrain_globals = OGRE_NEW TerrainGlobalOptions();
        // TODO: setup ?
    }

    ogre_terrain_group = 
        OGRE_NEW TerrainGroup(ogre_scenemgr, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
#ifdef USE_SAMPLE_ENDLESSWORLD
    ogre_terrain_group->setFilenameConvention(ENDLESS_TERRAIN_FILE_PREFIX, ENDLESS_TERRAIN_FILE_SUFFIX);
    ogre_terrain_group->setAutoUpdateLod( TerrainAutoUpdateLodFactory::getAutoUpdateLod(BY_DISTANCE) );
#endif
#ifdef USE_SAMPLE_TERRAIN
    ogre_terrain_group->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
    ogre_terrain_group->setResourceGroup("Terrain");
#endif

    terrain_group = ogre_terrain_group;

    // Configure global
    ogre_terrain_globals->setMaxPixelError( 8 );
    // testing composite map
    ogre_terrain_globals->setCompositeMapDistance( 3000 );
    //ogre_terrain_globals->setUseRayBoxDistanceCalculation(true);
    ogre_terrain_globals->getDefaultMaterialGenerator()->setLightmapEnabled( false );

    // FIXME: SceneManager, light, ...
    auto* l = ogre_scenemgr->getLight( "tstLight" );
    ogre_terrain_globals->setCompositeMapAmbient( ogre_scenemgr->getAmbientLight() ); 
    ogre_terrain_globals->setCompositeMapDiffuse( l->getDiffuseColour());    
    ogre_terrain_globals->setLightMapDirection( l->getDerivedDirection());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = ogre_terrain_group->getDefaultImportSettings();
    defaultimp.terrainSize = TERRAIN_SIZE;
    defaultimp.worldSize = TERRAIN_WORLD_SIZE;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures
    // see 'initBlendMaps below 
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100; // scale layer
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30; // scale layer
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200; // scale layer
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
////////////////////////////////////////////////////////////////////////////////

    // Paging setup
#ifndef USE_SAMPLE_TERRAIN
    ogre_page_manager = OGRE_NEW PageManager();
    // Since we're not loading any pages from .page files, we need a way just 
    // to say we've loaded them without them actually being loaded
    ogre_page_manager->setPageProvider(&dummy_page_provider);
    auto* camera = ogre_scenemgr->getCamera(  "Camera::ogre_camera" );
    ogre_page_manager->addCamera(camera);
    ogre_page_manager->setDebugDisplayLevel(0);
    ogre_terrain_paging = OGRE_NEW TerrainPaging(ogre_page_manager);
    ogre_paged_world = ogre_page_manager->createWorld();
    ogre_paged_world_section = ogre_terrain_paging->createWorldSection(ogre_paged_world, ogre_terrain_group, 
#ifdef USE_SAMPLE_ENDLESSWORLD
            400, 500, 
            ENDLESS_PAGE_MIN_X, ENDLESS_PAGE_MIN_Y, 
            ENDLESS_PAGE_MAX_X, ENDLESS_PAGE_MAX_Y);
#endif
#ifdef USE_SAMPLE_TERRAIN
            400, 500, 
            ENDLESS_PAGE_MIN_X, ENDLESS_PAGE_MIN_Y, 
            ENDLESS_PAGE_MAX_X, ENDLESS_PAGE_MAX_Y);
            //2000, 3000,
            //TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y,
            //TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y);
#endif
#endif

#ifdef USE_SAMPLE_ENDLESSWORLD
    perlin_noise = OGRE_NEW PerlinNoiseTerrainGenerator( 3.3, 2.2, 10, 128, 0.4 );
    ogre_paged_world_section->setDefiner( perlin_noise );
//		ogre_paged_world_section->setDefiner( OGRE_NEW SimpleTerrainDefiner );

        TerrainGroup::TerrainIterator ti = ogre_terrain_group->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }

#endif
#ifdef USE_SAMPLE_TERRAIN
    bool blankTerrain = false;
    for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
        for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
            defineTerrain(x, y, blankTerrain);
    // sync load since we want everything in place when we start
    ogre_terrain_group->loadAllTerrains(true);
    // ^FIXME:
    //  loadAllTerrains does something to our GL context/state, causing wrong output until
    //  OGRE::output called. the error is caused in OgreTerrainGroup::loadTerrainImpl when
    //  a work request is added to Ogre. The request handler is OgreTerrainGroup, but I 
    //  have not tracked the error there and further down.

    if (terrains_imported)
    {
        TerrainGroup::TerrainIterator ti = ogre_terrain_group->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    ogre_terrain_group->freeTemporaryResources();
#endif

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


//#ifdef USE_SAMPLE_TERRAIN
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

