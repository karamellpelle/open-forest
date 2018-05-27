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

namespace batb
{

namespace forest
{


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

    ////////////////////////////////////////////////////////////////////////////////
    // create SceneManager
    // see http://www.ogre3d.org/tikiwiki/SceneManagersFAQ for managers
    //forest.ogre_scenemanager = batb->ogre->ogre_root->createSceneManager( "DefaultSceneManager" );
    forest.ogre_scenemanager = batb->ogre->ogre_root->createSceneManager();

    // see Ogre::ApplicationContext::createDummyScene()
    // FIXME: remove upon deletion of forest::World? see Ogre::SampleBrowser::runSample()/Ogre::ApplicationContext::create/destroyDummyScene()
    batb->ogre->ogre_shader_generator->addSceneManager( forest.ogre_scenemanager );

    auto scnMgr = forest.ogre_scenemanager;
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5)); 

    Light* light = scnMgr->createLight("MainLight");
    SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);

    //scnMgr->setSkyBox(true, "Examples/MorningSkyBox");

    //! [newlight]

    //! [lightpos]
    lightNode->setPosition(20, 80, 50);
    //! [lightpos]

    //! [camera]
    SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    camNode->setPosition(0, 0, 140);
    //! [cameramove]
        // and tell it to render into the main window
    batb->ogre->ogre_renderwindow->addViewport(cam);

//! [entity1]
    Entity* ogreEntity = scnMgr->createEntity("ogrehead.mesh");
    //! [entity1]

    //! [entity1node]
    SceneNode* ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    //! [entity1node]

    //! [entity1nodeattach]
    ogreNode->attachObject(ogreEntity);
    //! [entity1nodeattach]

    //! [cameramove]
    camNode->setPosition(0, 47, 222);
    forest.camera.ogre_camera = cam;
    forest.camera.ogre_scenenode = camNode;
    ////////////////////////////////////////////////////////////////
    // create a view into the Ogre scene, a Camera!
    // it has to be connected to a SceneNode (new in version 1.10)

#if 0
    Ogre::Camera* cam = forest.ogre_scenemanager->createCamera( "Camera::ogre_camera" );
    Ogre::SceneNode* cam_node = forest.ogre_scenemanager->getRootSceneNode()->createChildSceneNode();
    cam_node->attachObject( cam );
    cam->setNearClipDistance(0.1);
    cam->setFarClipDistance(50000);
    if ( batb->ogre->ogre_root->getRenderSystem()->getCapabilities()->hasCapability( RSC_INFINITE_FAR_PLANE ) )
    {
        // enable infinite far clip distance if we can
        cam->setFarClipDistance( 0 );   
    }


    ////////////////////////////////////////////////////////////////
    // load terrain (based on Ogre sample)
    forest.terrain.ogre_scenemanager = forest.ogre_scenemanager;
    forest.terrain.ogre_viewport = forest.ogre_viewport;
    forest.terrain.load( YAML::Node() ); // TODO: yaml[ "xxx" ]


    ////////////////////////////////////////////////////////////////
    // set weater TODO
    forest.weather.load( YAML::Node() ); // TODO: yaml[ "xxx" ]
    forest.ogre_scenemanager->setSkyBox( true, "Examples/CloudyNoonSkyBox" ); 
    
    // setup Forest::Camera
    Ogre::Vector3 terrain_pos(1000,0,5000);
    cam_node->setPosition(terrain_pos + Vector3(1683, 50, 2116));
    cam_node->lookAt( Ogre::Vector3(1963, 50, 1660), Ogre::Node::TransformSpace::TS_WORLD );
    forest.camera.ogre_camera = cam;
    forest.camera.ogre_scenenode = cam_node;
    // ^ FIXME: this is redudant because we use DTMovable
    // place camera
    float_t height = 50 + forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( Vector3::ZERO );
    forest.camera.move.pos = glm::vec4( 0, height, 0, 1 );


    // create Viewport, the 2D target of Camera
    forest.ogre_viewport = batb->ogre->ogre_renderwindow->addViewport( forest.camera.ogre_camera );
    forest.ogre_viewport->setClearEveryFrame( false, 0 ); 
    forest.ogre_viewport->setBackgroundColour( ColourValue( 0.0, 0.6, 0.2 )); // tmp
#endif

    // end ogre frame
    batb->gl->ogreEnd();

}


void WorldLoader::unload(World& forest)
{
    batb->log << "FIXME: unload forest::World!" << std::endl;
}



} // namespace forest

} // namespace batb

