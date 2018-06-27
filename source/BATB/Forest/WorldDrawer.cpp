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
#include "BATB/Run/World.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/WorldDrawer.hpp"
#include "BATB/OGRE.hpp"
#include "BATB/OGRE/helpers.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/Value/Forest.hpp"
#include "OgreRoot.h"
#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreNode.h"
#include "OgreSceneNode.h"
#include "OgreRenderTarget.h"

#define KEEP_CAMERA_ABOVE_GROUND

namespace batb
{

namespace forest
{

WorldDrawer::WorldDrawer() 
{

}

void WorldDrawer::init(World* forest)
{
    // since we work directly with references to World, we can't initialize again.
    if ( forest_ ) throw std::runtime_error( "init() non-empty WorldDrawer" );

    ////////////////////////////////////////////////////////////////
    // create a view into the Ogre scene, a Camera!
    //
    ogre_camera_ = forest->ogre_scenemanager->createCamera( "ogre_camera" );
    ogre_camera_scenenode_ = forest->ogre_scenemanager->getRootSceneNode()->createChildSceneNode();
    ogre_camera_scenenode_->setFixedYawAxis( true ); // strange stuff happens without this. FIXME
    ogre_camera_scenenode_->attachObject( ogre_camera_ );
    ogre_camera_->setNearClipDistance(0.1);
    // enable infinite far clip distance if we can
    if ( Ogre::Root::getSingletonPtr()->getRenderSystem()->getCapabilities()->hasCapability( Ogre::RSC_INFINITE_FAR_PLANE ) )
    {
        ogre_camera_->setFarClipDistance( 0 );   
    }
    else
    {
        ogre_camera_->setFarClipDistance(50000);
    }
        
    ////////////////////////////////////////////////////////////////
    // place camera above (0, 0) of XZ-plane 
    float_t height = 50 + forest->terrain.ogre_terrain_group->getHeightAtWorldPosition( Ogre::Vector3::ZERO );
    camera_.move.pos = glm::vec4( 0, height, 0, 1 );


    // always work on this World
    forest_ = forest;

}

void WorldDrawer::draw(Ogre::RenderTarget* ogre_rendertarget)
{
    if ( ogre_rendertarget == nullptr )
    {
        return;
    }

    using namespace Ogre;

    // every Rendertarget needs a Viewport in order to be draw to.
    // we have to create one if we haven't added one to the target.
    // the Viewport is the 2D target of Camera. 
    constexpr int camera_zorder = 0;
    Ogre::Viewport* ogre_viewport = nullptr;
    if ( ogre_rendertarget->hasViewportWithZOrder( camera_zorder ) )
    {
        ogre_viewport = ogre_rendertarget->getViewportByZOrder( camera_zorder );
    }
    else
    {
        ogre_viewport = ogre_rendertarget->addViewport( ogre_camera_, camera_zorder );
        ogre_viewport->setClearEveryFrame( false, 0 ); 
    }
    


    // set aspect ratio
    float_t aspect = (float_t)( ogre_rendertarget->getWidth() ) / (float_t)( ogre_rendertarget->getHeight() );
    ogre_camera_->setAspectRatio( aspect );


    ////////////////////////////////////////////////////////////////////////////////
    // set Ogre::Camera from our Camera
    // FIXME: use setOrientation using Aim insted of setPosition/setDirection
    auto aim = camera_.move.aim;
    ogre_camera_scenenode_->setPosition(  ogre::cast_( aim[3]  ) );
    ogre_camera_scenenode_->setDirection( ogre::cast_( aim[2] ) , SceneNode::TransformSpace::TS_WORLD ); 
    //ogre_camera_scenenode_->setDirection( ogre::cast_( -aim[2] ) , SceneNode::TransformSpace::TS_WORLD ); // camera looks in negative Z direction
    //Ogre::Quaternion quat = ogre::cast( glm::quat_cast( forest.camera.move.aim ) ); 
    //forest.camera.ogre_scenenode->setOrientation( quat ); 

    // render 3D view from camera into ogre_rendertarget
    // NOTE: see https://ogrecave.github.io/ogre/api/1.11/class_ogre_1_1_render_target.html#ad0b724596d2b9e278293aee6e55a5273
    ogre_rendertarget->_updateViewport( ogre_viewport, true );

    ////////////////////////////////////////////////////////////////////////////////

}

void WorldDrawer::step(tick_t tick)
{
    // make sure we can handle very fast framerate (tick == tick_)
    while ( tick_ + value::dt <= tick )
    {

        ////////////////////////////////////////////////////////////////////////////////
        // move camera
        // 
        // no need for acceleration. if we decide to implement that, we must step in 
        // small and equal dt steps (i.e. value::dt)
        //camera_.move.acc[3] = 1.0;
        //camera_.move.vel += (float)( dt ) * move.acc;
        camera_.move.vel[3] = 1.0;

        camera_.move.pos += (float)( value::dt ) * camera_.move.vel;
        camera_.move.pos[3] = 1.0;

        camera_.move.compute();

#ifdef KEEP_CAMERA_ABOVE_GROUND
        {
        auto pos = camera_.move.pos;
        auto terrain_h = forest_->terrain.ogre_terrain_group->getHeightAtWorldPosition( Ogre::Vector3( pos.x, pos.y, pos.z ) );
        camera_.move.pos.y = std::max( pos.y, 1 + terrain_h ); 
        }
#endif



        tick_ += value::dt;
    }
}


void WorldDrawer::cameraFree()
{

}

void WorldDrawer::aiming(const AimControllable::Aiming& aim)
{
    // rotation
    camera_.move.aim[0] = aim.aim[0];
    camera_.move.aim[1] = aim.aim[1];
    camera_.move.aim[2] = aim.aim[2];

    float_t x_timed = std::min( 1.0, aim.ticks_x / value::forestCameraMoveXTicks );
    float_t z_timed = std::min( 1.0, aim.ticks_z / value::forestCameraMoveZTicks );

    float x_speed = std::max( value::forestCameraMoveXMin, x_timed * value::forestCameraMoveXMax ) * aim.move_x;
    float z_speed = std::max( value::forestCameraMoveZMin, z_timed * value::forestCameraMoveZMax ) * aim.move_z;

    // velocity for position
    camera_.move.vel = x_speed * aim.aim[0] + z_speed * aim.aim[2];
}

Aim WorldDrawer::getAim() const
{
    return camera_.move;
}


} // namespace forest

} // namespace batb



