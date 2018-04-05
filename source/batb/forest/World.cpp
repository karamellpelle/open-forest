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
#include <random>
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreTerrainGroup.h"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb/forest/Camera.hpp"
#include "batb/forest/Runner.hpp"
#include "batb/run.hpp"
#include "batb/BATB.hpp"


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

Runner* World::addRunner(run::Player* player)
{

    // create Runner in rigid memory (pointers valid for std::forward_list!)
    runners.push_front( Runner( *this, player ) );
    auto* runner = &runners.front();
    
    runner->reset( YAML::Node() ); // TODO: definition

    return runner;
}

WorldLoader::WorldLoader(BATB& b) : batb( b )
{

}


void WorldLoader::load(World& forest, const YAML::Node& yaml)
{

    ////////////////////////////////////////////////////////////////////////////////
    // setup AL

    forest.al_listener = batb.al.al_context.getListener();
    // TODO: create alure::Buffer's from defines in .yaml file.
    
    using namespace Ogre;
    // TODO: delete old before loading
    // TODO: use state
    // TODO: 
    //  * add demo resources (demo_begin)
    gl::begin_ogre();


    {
        // TODO: use yaml above
        YAML::Node yaml = YAML::LoadFile( file::static_data( "batb/forest/ogre.yaml" ) );

        // add resources for demo
        // TODO: find out how to use ResourceGroupManager::initialiseAllResourceGroups()
        if ( YAML::Node resources = yaml[ "resources" ] )
        {
            batb.ogre.addResourceLocation( yaml["resources"] );
        }
        else
        {
            batb.log << "forest::WorldLoader: no resources defined!" << std::endl;
        }

    } 
     

    ////////////////////////////////////////////////////////////////////////////////
    // create SceneManager
    // see http://www.ogre3d.org/tikiwiki/SceneManagersFAQ for managers
    forest.ogre_scenemgr = batb.ogre.ogre_root->createSceneManager( "DefaultSceneManager" );

    // create a view into scene, a Camera!
    forest.camera.ogre_camera = forest.ogre_scenemgr->createCamera( "Camera::ogre_camera" );

    //
    // create Viewport, the 2D target of Camera
    forest.ogre_viewport = batb.ogre.ogre_renderwindow->addViewport( forest.camera.ogre_camera );
    forest.ogre_viewport->setClearEveryFrame( false, 0 ); // TODO: remove 0??


    // set weater
    //forest.weather.load( yaml[ "Weather" ] );
    forest.weather.load( YAML::Node() );


    // TODO: setup light/... based on forest::Weather
#ifdef USE_SAMPLE_ENDLESSWORLD
    forest.ogre_scenemgr->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 4000, 10000);
#endif
#ifdef USE_SAMPLE_TERRAIN
    forest.ogre_scenemgr->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 10000, 25000); // Terrain
#endif

    //LogManager::getSingleton().setLogDetail(LL_BOREME); // ??

    Light* l = forest.ogre_scenemgr->createLight("tstLight");
    l->setType(Light::LT_DIRECTIONAL);
    Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
    l->setDirection( lightdir );
    l->setDiffuseColour(ColourValue::White);
#ifdef USE_SAMPLE_ENDLESSWORLD
    l->setSpecularColour(ColourValue(0.1, 0.1, 0.1));
#endif
#ifdef USE_SAMPLE_TERRAIN
    l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
#endif
    forest.ogre_scenemgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));


    // set terrain
    forest.terrain.ogre_scenemgr = forest.ogre_scenemgr;
    forest.terrain.ogre_viewport = forest.ogre_viewport;
    //forest.terrain.load( yaml[ "Terrain" ] );
    forest.terrain.load( YAML::Node() );


    // TODO: before terrain.load!
    forest.ogre_scenemgr->setSkyBox( true, "Examples/CloudyNoonSkyBox" ); 

#ifdef USE_SAMPLE_TERRAIN
Ogre::Vector3 terrain_pos(1000,0,5000);
    forest.camera.ogre_camera->setPosition(terrain_pos + Vector3(1683, 50, 2116));
    forest.camera.ogre_camera->lookAt(Vector3(1963, 50, 1660));
    forest.camera.ogre_camera->setNearClipDistance(0.1);
    forest.camera.ogre_camera->setFarClipDistance(50000);
#endif
#ifdef USE_SAMPLE_ENDLESSWORLD
    // view
    Vector3 terrain_pos;
    Vector3 forestCenter(
            (ENDLESS_PAGE_MAX_X+ENDLESS_PAGE_MIN_X) / 2 * TERRAIN_WORLD_SIZE,
            0,
            -(ENDLESS_PAGE_MAX_Y+ENDLESS_PAGE_MIN_Y) / 2 * TERRAIN_WORLD_SIZE
            );
    forest.camera.ogre_camera->setPosition(terrain_pos +forestCenter);
    forest.camera.ogre_camera->lookAt(terrain_pos);
    forest.camera.ogre_camera->setNearClipDistance(0.1);
    forest.camera.ogre_camera->setFarClipDistance(50000);

#endif
    if (batb.ogre.ogre_root->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
    {
        forest.camera.ogre_camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
    
    // place camera
    float_t height = 50 + forest.terrain.ogre_terrain_group->getHeightAtWorldPosition( Vector3::ZERO );
    forest.camera.move.pos = glm::vec4( 0, height, 0, 1 );

    gl::end_ogre();
}


void WorldLoader::unload(World& forest)
{
    batb.log << "FIXME: unload forest::World!" << std::endl;
}



} // namespace forest

} // namespace batb



