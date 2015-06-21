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
#include "batb/tmp/ogre.hpp"
#include "batb.hpp"
#include "batb/ogre.hpp"
#include "batb/run/World.hpp"


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
#include "batb/tmp/ogre/PerlinNoiseTerrainGenerator.h"

namespace batb
{



namespace tmp
{

namespace ogre
{


using namespace Ogre;

// max range for a int16
#define ENDLESS_TERRAIN_FILE_PREFIX String("EndlessWorldTerrain")
#define ENDLESS_TERRAIN_FILE_SUFFIX String("dat")
#define ENDLESS_PAGE_MIN_X (-0x7FFF)
#define ENDLESS_PAGE_MIN_Y (-0x7FFF)
#define ENDLESS_PAGE_MAX_X 0x7FFF
#define ENDLESS_PAGE_MAX_Y 0x7FFF
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513
#define HOLD_LOD_DISTANCE 3000.0


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
static Vector3 terrain_pos;

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
void iterate_terrain(BATB& batb, run::World& world);

void demo_begin(BATB& batb)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    if ( tmp_empty )
    {
        // begin GL state for Ogre
        gl::begin_ogre();

        YAML::Node yaml = YAML::LoadFile( file::static_data( "tmp/ogre.yaml" ) );

        // add resources for demo
        // TODO: find out how to use ResourceGroupManager::initialiseAllResourceGroups()
        if ( YAML::Node resources = yaml[ "resources" ] )
        {
            // iterate over group names
            for (auto i = std::begin( resources ); i != std::end( resources ); ++i )
            {
                // NOTE: resources is a map, not a list. and 'i' is a key. 
                //       this is yaml stuff (scalar, list, map)
                YAML::Node group = i->first;
                std::string name = group.as<std::string>();
                
                batb.log << "batb::tmp::ogre: adding items to resource group '" << name << "':\n";
                // iterate over defined content for that group
                for (auto j = std::begin( i->second ); j != std::end( i->second ); ++j )
                {
                    batb.log << "  ";

                    YAML::Node resource = *j;
                    if ( resource[ "type" ] && resource[ "path" ] )
                    {
                        std::string type = resource[ "type" ].as<std::string>();
                        std::string path = resource[ "path" ].as<std::string>();
                        
                        batb.log << path;

                        // add resource item
                        try
                        {
                            batb.ogre.root->addResourceLocation(  file::static_data( path ), type, name );
                        }
                        catch (Ogre::Exception& e)
                        {
                            batb.log << " (" << e.what() << ")";
                        }

                    }
                    else
                    {
                        batb.log << "(invalid item definition)";
                    }

                    batb.log << "\n";
                }
            }
        }
        else
        {
            throw std::runtime_error( "batb::tmp::ogre: no 'resources' defined in config" );
        }
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        ////////////////////////////////////////////////////////////////////////////////
        // create SceneManager
        // see http://www.ogre3d.org/tikiwiki/SceneManagersFAQ for managers
        scenemgr = batb.ogre.root->createSceneManager( "DefaultSceneManager" );
        
        // create a view into scene, a Camera!
        camera = scenemgr->createCamera( "PlayerCam" );
      
        // create Viewport, the 2D target of Camera
        viewport = batb.ogre.renderwindow->addViewport( camera );
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

void demo_iterate(BATB& batb, run::World& world)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // no need for gl::ogre_begin/end since no rendering?

    float_t aspect = world.scene.shape.wth / world.scene.shape.hth;

    camera->setAspectRatio( aspect );
    // TODO: camera->setProjection( world.scene.proj3D );

    //iterate_head(batb, world);
    iterate_terrain(batb, world);

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

    // create object, which becomes a singleton
    terrain_globals = OGRE_NEW TerrainGlobalOptions();


    MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
    MaterialManager::getSingleton().setDefaultAnisotropy(7);

    scenemgr->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 4000, 10000);

    //LogManager::getSingleton().setLogDetail(LL_BOREME); // ??

    Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();

    Light* l = scenemgr->createLight("tstLight");
    l->setType(Light::LT_DIRECTIONAL);
    l->setDirection(lightdir);
    l->setDiffuseColour(ColourValue::White);
    l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));

    scenemgr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));

    terrain_group = OGRE_NEW TerrainGroup(scenemgr, Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    terrain_group->setFilenameConvention(ENDLESS_TERRAIN_FILE_PREFIX, ENDLESS_TERRAIN_FILE_SUFFIX);
    //terrain_group->setOrigin( Vector3::ZERO );
    terrain_group->setAutoUpdateLod( TerrainAutoUpdateLodFactory::getAutoUpdateLod(BY_DISTANCE) );

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
    terrain_globals->setCompositeMapDiffuse(l->getDiffuseColour());
    terrain_globals->setLightMapDirection(l->getDerivedDirection());

    // Configure default import settings for if we use imported image
    Terrain::ImportData& defaultimp = terrain_group->getDefaultImportSettings();
    defaultimp.terrainSize = TERRAIN_SIZE;
    defaultimp.worldSize = TERRAIN_WORLD_SIZE;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
////////////////////////////////////////////////////////////////////////////////

    // Paging setup
    page_manager = OGRE_NEW PageManager();
    // Since we're not loading any pages from .page files, we need a way just 
    // to say we've loaded them without them actually being loaded
    page_manager->setPageProvider(&dummy_page_provider);
    page_manager->addCamera(camera);
    page_manager->setDebugDisplayLevel(0);
    terrain_paging = OGRE_NEW TerrainPaging(page_manager);
    paged_world = page_manager->createWorld();
    paged_world_section = terrain_paging->createWorldSection(paged_world, terrain_group, 400, 500, 
            ENDLESS_PAGE_MIN_X, ENDLESS_PAGE_MIN_Y, 
            ENDLESS_PAGE_MAX_X, ENDLESS_PAGE_MAX_Y);

    perlin_noise = OGRE_NEW PerlinNoiseTerrainGenerator( 3.3, 2.2, 10, 128, 1.2 );
    paged_world_section->setDefiner( perlin_noise );
//		paged_world_section->setDefiner( OGRE_NEW SimpleTerrainDefiner );

    terrain_group->freeTemporaryResources();

    scenemgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");

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

		camera->setFarClipDistance(50000);

		if (batb.ogre.root->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
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


void iterate_terrain(BATB& batb, run::World& world)
{
    terrain_group->autoUpdateLodAll(false, Any( Real(HOLD_LOD_DISTANCE) ));
    tick_t tick = world.tick;
    float_t x,z;
    cossin( 0.1 * tick, x, z );
    float_t y = sin( tick * 3 );

    Vector3 pos( 0, 400, 0 );

    camera->setPosition( pos );
    Ogre::Vector3 dir( x, -0.14, z );
    dir.normalise();

    camera->setDirection( dir );
}




} // namespace ogre

} // namespace tmp


} // namespace batb




