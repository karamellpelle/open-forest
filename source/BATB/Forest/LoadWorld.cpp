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
#include "BATB/Forest/LoadWorld.hpp"
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


LoadWorld::LoadWorld(BATB* b)
{
    batb = b;
}


void LoadWorld::load(const YAML::Node& yaml, World* forest)
{

    ////////////////////////////////////////////////////////////////////////////////
    // setup AL

    forest->alure_listener = batb->al->alure_context.getListener();
    


    // TODO: delete old before loading
    // TODO: use state
    // TODO: 
    batb->gl->ogreBegin();


    ////////////////////////////////////////////////////////////////////////////////
    // Ogre
    //
    // see:
    //  * tutorial (https://ogrecave.github.io/ogre/api/1.11/tut__first_scene.html)
    //
    ////////////////////////////////////////////////////////////////////////////////

    {
        // TODO: use yaml above
        YAML::Node yaml = YAML::LoadFile( file::static_data( "BATB/Forest/ogre.yaml" ) );

        // add resources for demo
        // TODO: find out how to use ResourceGroupManager::initialiseAllResourceGroups()
        if ( YAML::Node resources = yaml[ "resources" ] )
        {
            batb->ogre->addResourceGroupsAndInit( yaml["resources"] );
        }
        else
        {
            batb->log << "forest::LoadWorld: no resources defined!" << std::endl;
        }

    } 

    using namespace Ogre;



    ////////////////////////////////////////////////////////////////
    // create SceneManager
    forest->ogre_scenemanager = batb->ogre->ogre_root->createSceneManager();
   


    // should be done right after createSceneManager(). see https://ogrecave.github.io/ogre/api/1.11/_shadows.html
    // but need camera first in the mehtod's implementation
    // it's CPU intensive!
    //ogreShadows( forest );

    ////////////////////////////////////////////////////////////////
    // load terrain (based on Ogre sample)
    //
    forest->terrain.ogre_scenemanager = forest->ogre_scenemanager;
    load( YAML::Node(), &forest->terrain ); // TODO: yaml[ "xxx" ]


    ////////////////////////////////////////////////////////////////
    // set weater TODO
    //
    //forest->weather.load( YAML::Node() ); // TODO: yaml[ "xxx" ]
    forest->weather.setDay();
    //forest->weather.setNight();



    // tmp: test light
#if 0
        auto* ogre_headlamp = forest->ogre_scenemanager->createLight();
        ogre_headlamp->setType( Light::LT_SPOTLIGHT );
        ogre_headlamp->setSpotlightRange( Degree(20), Degree( 30 ));

        ogre_headlamp->setDiffuseColour( 0.0, 1.0, 0.0 );
        ogre_headlamp->setSpecularColour( 0.0, 0.0, 1.0 );

        // attach spotlight to runners head
        SceneNode* node = forest->ogre_scenemanager->createSceneNode(); // TODO: SceneNode of head
        node->setDirection( Vector3( 0, -1.0, 0 ).normalisedCopy() );

        node->setPosition(Vector3(0, height, 0));
        node->attachObject( ogre_headlamp );
#endif


    // end ogre frame
    batb->gl->ogreEnd();

}


void LoadWorld::unload(World& forest)
{
    batb->log << "FIXME: unload forest::World!" << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// load Terrain, based on Ogre sample

// TODO: enable paging of terrain?
//#define PAGING_ENABLE

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

void LoadWorld::load(const YAML::Node& yaml, Terrain* terrain)
{
    using namespace Ogre;

    ////////////////////////////////////////////////////////////////////////////////
    // Sample_Terrain::setupContent()

    // create the only TerrainGlobalOptions, if not present
    // see https://ogrecave.github.io/ogre/api/1.11/class_ogre_1_1_terrain_global_options.html
    if ( terrain->ogre_terrain_globals == nullptr )
    {
        terrain->ogre_terrain_globals = new TerrainGlobalOptions();
        // TODO: setup ?
    }

    MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
    MaterialManager::getSingleton().setDefaultAnisotropy(7);


    terrain->ogre_terrain_group = OGRE_NEW TerrainGroup(terrain->ogre_scenemanager, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    terrain->ogre_terrain_group->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
    terrain_group = terrain->ogre_terrain_group; // since helper functions below are not class members


    ////////////////////////////////////////////////////////////////
    // Sample_Terrain::configureTerrainDefaults(Light* l)

    // Configure global
    terrain->ogre_terrain_globals->setMaxPixelError( 8 );
    // testing composite map
    terrain->ogre_terrain_globals->setCompositeMapDistance( 3000 );
    //mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
    //mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
    //mTerrainGlobals->setLightMapSize(256);

    // Disable the lightmap for OpenGL ES 2.0. The minimum number of samplers allowed is 8(as opposed to 16 on desktop).
    // Otherwise we will run over the limit by just one. The minimum was raised to 16 in GL ES 3.0.
    if (Ogre::Root::getSingletonPtr()->getRenderSystem()->getCapabilities()->getNumTextureUnits() < 9)
    {
        TerrainMaterialGeneratorA::SM2Profile* matProfile =
            static_cast<TerrainMaterialGeneratorA::SM2Profile*>( terrain->ogre_terrain_globals->getDefaultMaterialGenerator()->getActiveProfile());
        matProfile->setLightmapEnabled(false);
    }

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& terrain_import = terrain->ogre_terrain_group->getDefaultImportSettings();
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
    terrain->ogre_page_manager = OGRE_NEW PageManager();
    // Since we're not loading any pages from .page files, we need a way just 
    // to say we've loaded them without them actually being loaded
    page_provider = new TerrainPageProvider(); // custom class
    terrain->ogre_page_manager->setPageProvider( page_provider );
    terrain->ogre_page_manager->addCamera( terrain->ogre_scenemanager->getCamera(  "ogre_camera" ) );
    terrain->ogre_page_manager->setDebugDisplayLevel( 0 );
    terrain->ogre_terrain_paging = OGRE_NEW TerrainPaging( terrain->ogre_page_manager );
    terrain->ogre_paged_world = terrain->ogre_page_manager->createWorld();
    terrain->ogre_paged_world_section = terrain->ogre_terrain_paging->createWorldSection(terrain->ogre_paged_world, terrain->ogre_terrain_group, 2000, 3000,
                                           TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y,
                                           TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y);

#else
    bool blankTerrain = false;
    for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
        for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
            defineTerrain(x, y, blankTerrain);
    // sync load since we want everything in place when we start
    terrain->ogre_terrain_group->loadAllTerrains(true);
#endif

    if (terrains_imported)
    {
        TerrainGroup::TerrainIterator ti = terrain->ogre_terrain_group->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }
    terrain->ogre_terrain_group->updateDerivedData();

    terrain->ogre_terrain_group->freeTemporaryResources();

}



////////////////////////////////////////////////////////////////////////////////
// 
    void defineTerrain(long x, long y, bool flat )
    {
        using namespace Ogre;
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

    void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
    {
        using namespace Ogre;

        img.load("terrain.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        if (flipX)
            img.flipAroundY();
        if (flipY)
            img.flipAroundX();
    }

    void initBlendMaps(Ogre::Terrain* terrain)
    {
        using namespace Ogre;

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

////////////////////////////////////////////////////////////////////////////////
// I dont know what this does; it's from the Ogre samples

static Ogre::ShadowCameraSetupPtr mPSSMSetup;

void LoadWorld::shadows(World* forest)
{
    using namespace Ogre;

   // below does not work, but this do
   forest->ogre_scenemanager->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    
#if 0
    Ogre::MaterialPtr buildDepthShadowMaterial(const Ogre::String& );

    bool enabled = true;
    bool depthShadows = false;

    // create global options if not present yet
    Terrain::ogre_terrain_globals = Terrain::ogre_terrain_globals ? Terrain::ogre_terrain_globals : new TerrainGlobalOptions();

    TerrainMaterialGeneratorA::SM2Profile* matProfile =
        static_cast<TerrainMaterialGeneratorA::SM2Profile*>( Terrain::ogre_terrain_globals->getDefaultMaterialGenerator()->getActiveProfile());
    matProfile->setReceiveDynamicShadowsEnabled(enabled);
#ifdef SHADOWS_IN_LOW_LOD_MATERIAL
    matProfile->setReceiveDynamicShadowsLowLod(true);
#else
    matProfile->setReceiveDynamicShadowsLowLod(false);
#endif

    //forest->ogre_scenemanager->setShadowTechnique( ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE );
    forest->ogre_scenemanager->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    forest->ogre_scenemanager->setShadowFarDistance(3000);
    // 3 textures per directional light (PSSM)
    forest->ogre_scenemanager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
    
    Ogre::Camera* cam = forest->camera.ogre_camera;
    
    if (!mPSSMSetup)
    {
        // shadow camera setup
        PSSMShadowCameraSetup* pssmSetup = new PSSMShadowCameraSetup();
        pssmSetup->setSplitPadding( cam->getNearClipDistance());
        pssmSetup->calculateSplitPoints(3,  cam->getNearClipDistance(), forest->ogre_scenemanager->getShadowFarDistance());
        pssmSetup->setOptimalAdjustFactor(0, 2);
        pssmSetup->setOptimalAdjustFactor(1, 1);
        pssmSetup->setOptimalAdjustFactor(2, 0.5);

        mPSSMSetup.reset(pssmSetup);
    }
    forest->ogre_scenemanager->setShadowCameraSetup(mPSSMSetup);

    if (depthShadows)
    {
        forest->ogre_scenemanager->setShadowTextureCount(3);
        forest->ogre_scenemanager->setShadowTextureConfig(0, 2048, 2048, PF_FLOAT32_R);
        forest->ogre_scenemanager->setShadowTextureConfig(1, 1024, 1024, PF_FLOAT32_R);
        forest->ogre_scenemanager->setShadowTextureConfig(2, 1024, 1024, PF_FLOAT32_R);
        forest->ogre_scenemanager->setShadowTextureSelfShadow(true);
        forest->ogre_scenemanager->setShadowCasterRenderBackFaces(true);

        //MaterialPtr houseMat = buildDepthShadowMaterial("fw12b.jpg");
        //for (EntityList::iterator i = mHouseList.begin(); i != mHouseList.end(); ++i)
        //{
        //    (*i)->setMaterial(houseMat);
        //}
    }
    else
    {
        forest->ogre_scenemanager->setShadowTextureCount(3);
        forest->ogre_scenemanager->setShadowTextureConfig(0, 2048, 2048, PF_X8B8G8R8);
        forest->ogre_scenemanager->setShadowTextureConfig(1, 1024, 1024, PF_X8B8G8R8);
        forest->ogre_scenemanager->setShadowTextureConfig(2, 1024, 1024, PF_X8B8G8R8);
        forest->ogre_scenemanager->setShadowTextureSelfShadow(false);
        forest->ogre_scenemanager->setShadowCasterRenderBackFaces(false);
        forest->ogre_scenemanager->setShadowTextureCasterMaterial(MaterialPtr());
    }

    matProfile->setReceiveDynamicShadowsDepth(depthShadows);
    matProfile->setReceiveDynamicShadowsPSSM(static_cast<PSSMShadowCameraSetup*>(mPSSMSetup.get()));
#endif

}

Ogre::MaterialPtr buildDepthShadowMaterial(const Ogre::String& textureName)
{
    using namespace Ogre;

    String matName = "DepthShadows/" + textureName;

    MaterialPtr ret = MaterialManager::getSingleton().getByName(matName);
    if (!ret)
    {
        MaterialPtr baseMat = MaterialManager::getSingleton().getByName("Ogre/shadow/depth/integrated/pssm");
        ret = baseMat->clone(matName);
        Pass* p = ret->getTechnique(0)->getPass(0);
        p->getTextureUnitState("diffuse")->setTextureName(textureName);

        Vector4 splitPoints;
        const PSSMShadowCameraSetup::SplitPointList& splitPointList =
            static_cast<PSSMShadowCameraSetup*>(mPSSMSetup.get())->getSplitPoints();
        for (int i = 0; i < 3; ++i)
        {
            splitPoints[i] = splitPointList[i];
        }
        p->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);
    }

    return ret;
}



} // namespace forest

} // namespace batb

