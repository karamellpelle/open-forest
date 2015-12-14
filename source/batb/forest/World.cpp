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
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"
#include "batb/forest.hpp"
#include "batb/forest/Terrain.hpp"
#include "batb/run.hpp"
#include "batb/BATB.hpp"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreTerrainGroup.h"
//#include "OgreSceneNode.h"
//#include "OgreViewport.h"
//#include "OgreRenderWindow.h"
//#include "OgreRoot.h"


namespace batb
{

namespace forest
{


Control* World::pushControl(const glm::vec2& point, const ControlDefinition& def)
{
    Control c;
    c.definition = def;
    c.pos = glm::vec4( point[0], point[1], 0.0, 1.0 );

    controls.push_front( c );

    return &controls.front();

}


WorldLoader::WorldLoader(BATB& b) : batb( b )
{

}


void WorldLoader::load(World& world, const YAML::Node& def)
{
    using namespace Ogre;
    // TODO: delete old before loading
    // TODO: use state
    // TODO: 
    //  * add demo resources (demo_begin)
    gl::begin_ogre();

        YAML::Node yaml = YAML::LoadFile( file::static_data( "demo/libs/ogre.yaml" ) );

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
                
                batb.log << "batb::demo::ogre: adding items to resource group '" << name << "':\n";
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
                            batb.ogre.ogre_root->addResourceLocation(  file::static_data( path ), type, name );
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
            throw std::runtime_error( "batb::demo::ogre: no 'resources' defined in config" );
        }
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        

    ////////////////////////////////////////////////////////////////////////////////
    // create SceneManager
    // see http://www.ogre3d.org/tikiwiki/SceneManagersFAQ for managers
    world.ogre_scenemgr = batb.ogre.ogre_root->createSceneManager( "DefaultSceneManager" );

    // create a view into scene, a Camera!
    world.camera.ogre_camera = world.ogre_scenemgr->createCamera( "Camera::ogre_camera" );

    //
    // create Viewport, the 2D target of Camera
    world.ogre_viewport = batb.ogre.ogre_renderwindow->addViewport( world.camera.ogre_camera );
    world.ogre_viewport->setClearEveryFrame( false, 0 ); // TODO: remove 0??


    // set weater
    //world.weather.load( yaml[ "Weather" ] );
    world.weather.load( YAML::Node() );


    // TODO: setup light/... based on forest::Weather
#ifdef USE_SAMPLE_ENDLESSWORLD
    world.ogre_scenemgr->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 4000, 10000);
#endif
#ifdef USE_SAMPLE_TERRAIN
    world.ogre_scenemgr->setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 10000, 25000); // Terrain
#endif

    //LogManager::getSingleton().setLogDetail(LL_BOREME); // ??

    Light* l = world.ogre_scenemgr->createLight("tstLight");
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
    world.ogre_scenemgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));


    // set terrain
    world.terrain.ogre_scenemgr = world.ogre_scenemgr;
    world.terrain.ogre_viewport = world.ogre_viewport;
    //world.terrain.load( yaml[ "Terrain" ] );
    world.terrain.load( YAML::Node() );


    // TODO: before terrain.load!
    world.ogre_scenemgr->setSkyBox( true, "Examples/CloudyNoonSkyBox" ); 

#ifdef USE_SAMPLE_TERRAIN
Ogre::Vector3 terrain_pos(1000,0,5000);
    world.camera.ogre_camera->setPosition(terrain_pos + Vector3(1683, 50, 2116));
    world.camera.ogre_camera->lookAt(Vector3(1963, 50, 1660));
    world.camera.ogre_camera->setNearClipDistance(0.1);
    world.camera.ogre_camera->setFarClipDistance(50000);
#endif
#ifdef USE_SAMPLE_ENDLESSWORLD
    // view
    Vector3 terrain_pos;
    Vector3 worldCenter(
            (ENDLESS_PAGE_MAX_X+ENDLESS_PAGE_MIN_X) / 2 * TERRAIN_WORLD_SIZE,
            0,
            -(ENDLESS_PAGE_MAX_Y+ENDLESS_PAGE_MIN_Y) / 2 * TERRAIN_WORLD_SIZE
            );
    world.camera.ogre_camera->setPosition(terrain_pos +worldCenter);
    world.camera.ogre_camera->lookAt(terrain_pos);
    world.camera.ogre_camera->setNearClipDistance(0.1);
    world.camera.ogre_camera->setFarClipDistance(50000);

#endif
    if (batb.ogre.ogre_root->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
    {
        world.camera.ogre_camera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
    ////////////////////////////////////////////////////////////////////////////////
    // models:
    Ogre::Entity* head0 = world.ogre_scenemgr->createEntity( "head0", "ogrehead.mesh" );
    Ogre::Entity* head1 = world.ogre_scenemgr->createEntity( "head1", "ogrehead.mesh" );
    Ogre::SceneNode* node0 = world.ogre_scenemgr->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* node1 = world.ogre_scenemgr->getRootSceneNode()->createChildSceneNode();
    node0->scale( 0.2, 0.2, 0.2 );
    node1->scale( 0.4, 0.4, 0.4 );
    Ogre::Vector3 pos0( 100, 0, -120 );
    Ogre::Vector3 pos1( 0, 0, -200 );
    float_t height0 = world.terrain.ogre_terrain_group->getHeightAtWorldPosition( pos0 );
    float_t height1 = world.terrain.ogre_terrain_group->getHeightAtWorldPosition( pos1 );
    node0->setPosition( pos0[0], height0 + 1, pos0[2] );
    node1->setPosition( pos1[0], height1 + 1, pos1[2] );
    node0->attachObject( head0 );
    node1->attachObject( head1 );
    ////////////////////////////////////////////////////////////////////////////////
    // see: SinbadCharacterController.h
    using namespace Ogre;
    auto* bodyNode = world.ogre_scenemgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * 5);
    bodyNode->scale( 3, 3, 3 );
    auto* bodyEnt = world.ogre_scenemgr->createEntity("SinbadBody", "Sinbad.mesh");
    bodyNode->attachObject(bodyEnt);
    Vector3 bodyPos( 40, 0, -160 );
    float_t bodyHeight = world.terrain.ogre_terrain_group->getHeightAtWorldPosition( bodyPos );
    bodyNode->setPosition( bodyPos[0], bodyHeight + 8, bodyPos[2] );

        // this is very important due to the nature of the exported animations
        //bodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);
        //String animNames[] =
        //{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
        //"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};
        //
        //// populate our animation list
        //for (int i = 0; i < NUM_ANIMS; i++)
        //{
        //    mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
        //    mAnims[i]->setLoop(true);
        //    mFadingIn[i] = false;
        //    mFadingOut[i] = false;
        //}
        //
        //// start off in the idle state (top and bottom together)
        //setBaseAnimation(ANIM_IDLE_BASE);
        //setTopAnimation(ANIM_IDLE_TOP);
        //
        //// relax the hands since we're not holding anything
        //mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);



    gl::end_ogre();
}

} // namespace forest

} // namespace batb



