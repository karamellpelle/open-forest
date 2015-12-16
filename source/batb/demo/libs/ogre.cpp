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
#include "OgreMaterialManager.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreEntity.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreViewport.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreWindowEventUtilities.h"
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

#include "batb.hpp"
#include "batb/ogre.hpp"
#include "batb/run/World.hpp"
#include "batb/demo/libs/ogre/PerlinNoiseTerrainGenerator.h"
#include "batb/demo/libs/ogre.hpp"



//#define USE_SAMPLE_ENDLESSWORLD
#define USE_SAMPLE_TERRAIN

namespace batb
{



namespace demo
{

namespace ogre
{


using namespace Ogre;

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
#include "macUtils.h"
#endif



Vector3 terrain_pos(1000,0,5000);
#define TERRAIN_FILE_PREFIX String("testTerrain")
#define TERRAIN_FILE_SUFFIX String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

#endif

bool terrains_imported = false;
void getTerrainImage(bool flipX, bool flipY, Image& img);
void initBlendMaps(Terrain* terrain);
void defineTerrain(long x, long y, bool flat = false);

////////////////////////////////////////////////////////////////////////////////
static bool tmp_empty = true;

// into ForestWorld:
static Ogre::SceneManager* scenemgr = nullptr;
static Ogre::Camera* camera = nullptr;
static Ogre::Viewport* viewport = nullptr;
// see Samples/EndlessTerrain:
static TerrainGlobalOptions* terrain_globals = nullptr; // only 1 per app; into BATB::Forest
static TerrainGroup* terrain_group = nullptr;
static TerrainPaging* terrain_paging = nullptr;
static PageManager* page_manager = nullptr;
static PagedWorld* paged_world = nullptr;
static TerrainPagedWorldSection* paged_world_section = nullptr;
static PerlinNoiseTerrainGenerator* perlin_noise;
#ifdef USE_SAMPLE_ENDLESSWORLD
static Vector3 terrain_pos;
#endif

////////////////////////////////////////////////////////////////////////////////
class DummyPageProvider : public PageProvider
{
public:
        bool prepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
        bool loadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
        bool unloadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
        bool unprepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
} dummy_page_provider;

void begin_head(BATB& );
void begin_terrain(BATB& );
void iterate_head(BATB& batb, run::World& world);
void iterate_terrain(BATB& batb, run::World& world, forest::World& forest);

void demo_begin(BATB& batb)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    if ( tmp_empty )
    {
        //
        // begin GL state for Ogre
        gl::begin_ogre();

        YAML::Node yaml = YAML::LoadFile( file::static_data( "demo/libs/ogre.yaml" ) );

        // add resources for demo
        // TODO: find out how to use ResourceGroupManager::initialiseAllResourceGroups()
        if ( YAML::Node resources = yaml[ "resources" ] )
        {
            batb.ogre.addResourceLocation( yaml["resources"] );
        }
        else
        {
            throw std::runtime_error( "batb::demo::ogre: no 'resources' defined in config" );
        }

        ////////////////////////////////////////////////////////////////////////////////
        // create SceneManager
        // see http://www.ogre3d.org/tikiwiki/SceneManagersFAQ for managers
        scenemgr = batb.ogre.ogre_root->createSceneManager( "DefaultSceneManager" );
        
        //// create a view into scene, a Camera!
        camera = scenemgr->createCamera( "PlayerCam" );
      
        // create Viewport, the 2D target of Camera
        viewport = batb.ogre.ogre_renderwindow->addViewport( camera );

        viewport->setClearEveryFrame( false, 0 ); // TODO: remove 0??

       
        ////////////////////////////////////////////////////////////////////////////////
        // create scene objects
        //begin_head(batb);
        begin_terrain(batb);

        // end GL state for Ogre
        gl::end_ogre();

    }
    tmp_empty = false;
}

void demo_iterate(BATB& batb, run::World& run, forest::World& forest)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // no need for gl::ogre_begin/end since no rendering?

    float_t aspect = run.scene.shape.wth / run.scene.shape.hth;

    camera->setAspectRatio( aspect );
    // TODO: camera->setProjection( run.scene.proj3D );

    //iterate_head(batb, run);
    iterate_terrain(batb, run, forest);

    // TODO: asset no need for GLContextGLFW switch
}



void begin_head(BATB& batb)
{
    Ogre::Entity* head = scenemgr->createEntity( "Head", "ogrehead.mesh" );
    Ogre::SceneNode* node = scenemgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject( head );

    scenemgr->setAmbientLight( Ogre::ColourValue(0.5, 0.5, 0.5));

    Ogre::Light* l = scenemgr->createLight( "MainLight" );
    l->setPosition( 20, 80, 50 );

    // position Camera
    camera->setPosition( Ogre::Vector3( 0, 0, 128 ) );
    camera->lookAt( Ogre::Vector3( 0, 0, -300 ) );

    // TODO: asset no need for GLContextGLFW switch
    
    // skybox:
    scenemgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");

        
    camera->setNearClipDistance( 5 );
}

void iterate_head(BATB& batb, run::World& world)
{
    tick_t tick = world.tick;
    float_t x,z;
    cossin( 0.5 * tick, x, z );
    //float_t y = sin( tick * 3 );
    float_t y = 50 + sin( tick * 3 );

    camera->setPosition( Ogre::Vector3( 128.0 * x, 64.0 * y, 128.0 * z ) );
    camera->lookAt( Ogre::Vector3( 0, 0, 0 ) );
}


void begin_terrain(BATB& batb)
{
// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Basic+Tutorial+3
// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Ogre+Terrain+System
// libs/ogre/Samples/EndlessWorld/include/EndlessWorld.h

#ifdef USE_SAMPLE_TERRAIN
    if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists("Terrain"))
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Terrain");
#endif

    // create object, which becomes a singleton
    // Terrain::setResourceGroup overrides this (??)
    terrain_globals = OGRE_NEW TerrainGlobalOptions();


    MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
    MaterialManager::getSingleton().setDefaultAnisotropy(7);

#ifdef USE_SAMPLE_ENDLESSWORLD
    scenemgr->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 4000, 10000);
#endif
#ifdef USE_SAMPLE_TERRAIN
    scenemgr->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 10000, 25000); // Terrain
#endif


    //LogManager::getSingleton().setLogDetail(LL_BOREME); // ??

    Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();

    Light* l = scenemgr->createLight("tstLight");
    l->setType(Light::LT_DIRECTIONAL);
    l->setDirection(lightdir);
    l->setDiffuseColour(ColourValue::White);
#ifdef USE_SAMPLE_ENDLESSWORLD
    l->setSpecularColour(ColourValue(0.1, 0.1, 0.1));
#endif
#ifdef USE_SAMPLE_TERRAIN
    l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
#endif

    scenemgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    terrain_group = OGRE_NEW TerrainGroup(scenemgr, Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
#ifdef USE_SAMPLE_ENDLESSWORLD
    terrain_group->setFilenameConvention(ENDLESS_TERRAIN_FILE_PREFIX, ENDLESS_TERRAIN_FILE_SUFFIX);
    terrain_group->setAutoUpdateLod( TerrainAutoUpdateLodFactory::getAutoUpdateLod(BY_DISTANCE) );
#endif
#ifdef USE_SAMPLE_TERRAIN
    terrain_group->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
    terrain_group->setResourceGroup("Terrain");
#endif


////////////////////////////////////////////////////////////////////////////////
// configureTerrainDefaults
//
    // Configure global
    terrain_globals->setMaxPixelError(8);
    // testing composite map
    terrain_globals->setCompositeMapDistance(3000);
    //terrain_globals->setUseRayBoxDistanceCalculation(true);
    terrain_globals->getDefaultMaterialGenerator()->setLightmapEnabled(false);

    terrain_globals->setCompositeMapAmbient(scenemgr->getAmbientLight());
    terrain_globals->setCompositeMapDiffuse(l->getDiffuseColour());    terrain_globals->setLightMapDirection(l->getDerivedDirection());

    // Configure default import settings for if we use imported image
    Terrain::ImportData& defaultimp = terrain_group->getDefaultImportSettings();
    defaultimp.terrainSize = TERRAIN_SIZE;
    defaultimp.worldSize = TERRAIN_WORLD_SIZE;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures
    // see 'initBlendMaps' below 
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
    page_manager = OGRE_NEW PageManager();
    // Since we're not loading any pages from .page files, we need a way just 
    // to say we've loaded them without them actually being loaded
    page_manager->setPageProvider(&dummy_page_provider);
    page_manager->addCamera(camera);
    page_manager->setDebugDisplayLevel(0);
    terrain_paging = OGRE_NEW TerrainPaging(page_manager);
    paged_world = page_manager->createWorld();
    paged_world_section = terrain_paging->createWorldSection(paged_world, terrain_group, 
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
    paged_world_section->setDefiner( perlin_noise );
//		paged_world_section->setDefiner( OGRE_NEW SimpleTerrainDefiner );

        TerrainGroup::TerrainIterator ti = terrain_group->getTerrainIterator();
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
    terrain_group->loadAllTerrains(true);
    // ^FIXME:
    //  loadAllTerrains does something to our GL context/state, causing wrong output until
    //  OGRE::output called. the error is caused in OgreTerrainGroup::loadTerrainImpl when
    //  a work request is added to Ogre. The request handler is OgreTerrainGroup, but I 
    //  have not tracked the error there and further down.

    if (terrains_imported)
    {
        TerrainGroup::TerrainIterator ti = terrain_group->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }
#endif

    terrain_group->freeTemporaryResources();

    scenemgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");

#ifdef USE_SAMPLE_TERRAIN
    camera->setPosition(terrain_pos + Vector3(1683, 50, 2116));
    camera->lookAt(Vector3(1963, 50, 1660));
    camera->setNearClipDistance(0.1);
    camera->setFarClipDistance(50000);
#endif
#ifdef USE_SAMPLE_ENDLESSWORLD
    // view
    Vector3 worldCenter(
            (ENDLESS_PAGE_MAX_X+ENDLESS_PAGE_MIN_X) / 2 * TERRAIN_WORLD_SIZE,
            0,
            -(ENDLESS_PAGE_MAX_Y+ENDLESS_PAGE_MIN_Y) / 2 * TERRAIN_WORLD_SIZE
            );
    camera->setPosition(terrain_pos +worldCenter);
    camera->lookAt(terrain_pos);
    camera->setNearClipDistance(0.1);
    camera->setFarClipDistance(50000);

#endif

    if (batb.ogre.ogre_root->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
    {
        camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
               
}

////////////////////////////////////////////////////////////////////////////////
// shutdown:
//

void demo_end(BATB& batb)
{
    if( terrain_paging )
    {
            OGRE_DELETE terrain_paging;
            page_manager->destroyWorld( paged_world );
            OGRE_DELETE page_manager;
    }

    if ( terrain_globals )
        OGRE_DELETE terrain_globals;

}


void iterate_terrain(BATB& batb, run::World& run, forest::World& forest)
{
    //terrain_group->autoUpdateLodAll(false, Any( Real(HOLD_LOD_DISTANCE) ));
    tick_t tick = run.tick;
    float_t x,z;
    cossin( 0.1 * tick, x, z );
    float_t y = sin( tick * 3 );

    //Vector3 pos( 0, 400, 0 );
    //camera->setPosition( pos );
    Ogre::Vector3 dir( x, -0.14, z );
    dir.normalise();
    //camera->setDirection( dir );
    if ( forest.runners.empty() )
    {
        std::cout << "runners.empty!!\n"; 
        camera->setDirection( dir );
    }
    else
    {
        forest::Runner runner = forest.runners.front();
        glm::mat4 aim = runner.aim;
        glm::vec4 z = aim[2];
        
        camera->setDirection( Ogre::Vector3( z[0], z[1], z[2] ) );

        glm::vec4 pos = runner.pos;
        camera->setPosition( Ogre::Vector3( pos[0], pos[1], pos[2] ) );

    }
    

}


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


} // namespace ogre

} // namespace demo


} // namespace batb




