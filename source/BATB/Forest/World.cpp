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
#include "BATB/Forest/Camera.hpp"
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


World::World(run::World& r) : terrain( *this ), weather( this ),  run( r )
{ 

}

Control* World::addControl(const ControlDefinition& def)
{

    // create Control in rigid memory (pointers valid for std::forward_list!)
    controls.push_front( Control( *this ) );
    auto* control = &controls.front();

    // place it in terrain
    control->reset( def );
   
    return control;
}

void World::removeControl(Control* control)
{
    // release data
    control->reset();

    // this invalidates 'control' !!
    auto pred = [=](const Control& c) { return &c == control; };
    controls.remove_if( pred ); // goes through whole list even if 'control' found

}

Runner* World::addRunner(Player* player)
{

    // create Runner in rigid memory (pointers valid for std::forward_list!)
    runners.push_front( Runner( *this, player ) );
    auto* runner = &runners.front();
    
    runner->reset( YAML::Node() ); // TODO: definition

    return runner;
}

WorldLoader::WorldLoader(BATB* b) : batb( b )
{

}


void WorldLoader::load(World& forest, const YAML::Node& yaml)
{

    ////////////////////////////////////////////////////////////////////////////////
    // setup AL

    forest.al_listener = batb->al->al_context.getListener();
    // TODO: create alure::Buffer's from defines in .yaml file.
    
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
            batb->log << "forest::WorldLoader: no resources defined!" << std::endl;
        }

    } 

    using namespace Ogre;



    ////////////////////////////////////////////////////////////////
    // create SceneManager
    forest.ogre_scenemanager = batb->ogre->ogre_root->createSceneManager();
   

    ////////////////////////////////////////////////////////////////
    // create a view into the Ogre scene, a Camera!
    Ogre::Camera* cam = forest.ogre_scenemanager->createCamera( "ogre_camera" );
    Ogre::SceneNode* cam_node = forest.ogre_scenemanager->getRootSceneNode()->createChildSceneNode();
    cam_node->setFixedYawAxis( true ); // strange stuff happens without this. FIXME
    cam_node->attachObject( cam );
    cam->setNearClipDistance(0.1);
    // enable infinite far clip distance if we can
    if ( batb->ogre->ogre_root->getRenderSystem()->getCapabilities()->hasCapability( RSC_INFINITE_FAR_PLANE ) )
    {
        cam->setFarClipDistance( 0 );   
    }
    else
    {
        cam->setFarClipDistance(50000);
    }
        
    forest.camera.ogre_camera = cam;
    forest.camera.ogre_scenenode = cam_node;

    // should be done right after createSceneManager(). see https://ogrecave.github.io/ogre/api/1.11/_shadows.html
    // but need camera first in the mehtod's implementation
    // it's CPU intensive!
    //ogreShadows( forest );

    ////////////////////////////////////////////////////////////////
    // load terrain (based on Ogre sample)
    //
    forest.terrain.ogre_scenemanager = forest.ogre_scenemanager;
    forest.terrain.ogre_viewport = forest.ogre_viewport;
    forest.terrain.load( YAML::Node() ); // TODO: yaml[ "xxx" ]


    ////////////////////////////////////////////////////////////////
    // set weater TODO
    //
    //forest.weather.load( YAML::Node() ); // TODO: yaml[ "xxx" ]
    forest.weather.setDay();
    //forest.weather.setNight();


    ////////////////////////////////////////////////////////////////
    // place camera at origo plane XZ
    float_t height = 50 + forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( Vector3::ZERO );
    forest.camera.move.pos = glm::vec4( 0, height, 0, 1 );

    // tmp: test light

    ////////////////////////////////////////////////////////////////
    // create Viewport, the 2D target of Camera
    // fix problem with externalWindow handling and resizing. see https://github.com/OGRECave/ogre/issues/774#issuecomment-392755242
    batb->ogre->ogre_renderwindow->windowMovedOrResized();
    forest.ogre_viewport = batb->ogre->ogre_renderwindow->addViewport( forest.camera.ogre_camera );
    forest.ogre_viewport->setClearEveryFrame( false, 0 ); 


    // end ogre frame
    batb->gl->ogreEnd();

}


void WorldLoader::unload(World& forest)
{
    batb->log << "FIXME: unload forest::World!" << std::endl;
}

static Ogre::ShadowCameraSetupPtr mPSSMSetup;

void WorldLoader::ogreShadows(World& forest)
{
    using namespace Ogre;

   // below does not work, but this do
   forest.ogre_scenemanager->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    
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

    //forest.ogre_scenemanager->setShadowTechnique( ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE );
    forest.ogre_scenemanager->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    forest.ogre_scenemanager->setShadowFarDistance(3000);
    // 3 textures per directional light (PSSM)
    forest.ogre_scenemanager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
    
    Ogre::Camera* cam = forest.camera.ogre_camera;
    
    if (!mPSSMSetup)
    {
        // shadow camera setup
        PSSMShadowCameraSetup* pssmSetup = new PSSMShadowCameraSetup();
        pssmSetup->setSplitPadding( cam->getNearClipDistance());
        pssmSetup->calculateSplitPoints(3,  cam->getNearClipDistance(), forest.ogre_scenemanager->getShadowFarDistance());
        pssmSetup->setOptimalAdjustFactor(0, 2);
        pssmSetup->setOptimalAdjustFactor(1, 1);
        pssmSetup->setOptimalAdjustFactor(2, 0.5);

        mPSSMSetup.reset(pssmSetup);
    }
    forest.ogre_scenemanager->setShadowCameraSetup(mPSSMSetup);

    if (depthShadows)
    {
        forest.ogre_scenemanager->setShadowTextureCount(3);
        forest.ogre_scenemanager->setShadowTextureConfig(0, 2048, 2048, PF_FLOAT32_R);
        forest.ogre_scenemanager->setShadowTextureConfig(1, 1024, 1024, PF_FLOAT32_R);
        forest.ogre_scenemanager->setShadowTextureConfig(2, 1024, 1024, PF_FLOAT32_R);
        forest.ogre_scenemanager->setShadowTextureSelfShadow(true);
        forest.ogre_scenemanager->setShadowCasterRenderBackFaces(true);

        //MaterialPtr houseMat = buildDepthShadowMaterial("fw12b.jpg");
        //for (EntityList::iterator i = mHouseList.begin(); i != mHouseList.end(); ++i)
        //{
        //    (*i)->setMaterial(houseMat);
        //}
    }
    else
    {
        forest.ogre_scenemanager->setShadowTextureCount(3);
        forest.ogre_scenemanager->setShadowTextureConfig(0, 2048, 2048, PF_X8B8G8R8);
        forest.ogre_scenemanager->setShadowTextureConfig(1, 1024, 1024, PF_X8B8G8R8);
        forest.ogre_scenemanager->setShadowTextureConfig(2, 1024, 1024, PF_X8B8G8R8);
        forest.ogre_scenemanager->setShadowTextureSelfShadow(false);
        forest.ogre_scenemanager->setShadowCasterRenderBackFaces(false);
        forest.ogre_scenemanager->setShadowTextureCasterMaterial(MaterialPtr());
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

