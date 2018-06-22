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
#include "OgreSingleton.h"
#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreNode.h"
#include "OgreSceneNode.h"
#include "OgreRenderTarget.h"


namespace batb
{

namespace forest
{

WorldDrawer::WorldDrawer() 
{


}

void WorldDrawer::init(World* f)
{
    forest_ = f;


}

void WorldDrawer::draw(Ogre::RenderTarget* ogre_rendertarget)
{

    using namespace Ogre;

    // TMP:
    ogre_camera_ = forest_->camera.ogre_camera;
    ogre_camera_scenenode_ = forest_->camera.ogre_scenenode;


    float_t aspect = (float_t)( ogre_rendertarget->getWidth() ) / (float_t)( ogre_rendertarget->getHeight() );
    ogre_camera_->setAspectRatio( aspect );


    //assert( aim_ );

    ////////////////////////////////////////////////////////////////////////////////
    // set camera from its DTMovable 
    //Aim aim = *aim_;
    auto aim = forest_->camera.move.aim; // tmp
    ogre_camera_scenenode_->setPosition(  ogre::cast_( aim[3]  ) );
    ogre_camera_scenenode_->setDirection( ogre::cast_( aim[2] ) , SceneNode::TransformSpace::TS_WORLD ); 
    //ogre_camera_scenenode_->setDirection( ogre::cast_( -aim[2] ) , SceneNode::TransformSpace::TS_WORLD ); // camera looks in negative Z direction
    // FIXME: use setOrientation using Aim insted of setPosition/setDirection
    //Ogre::Quaternion quat = ogre::cast( glm::quat_cast( forest.camera.move.aim ) ); 
    //forest.camera.ogre_scenenode->setOrientation( quat ); 

    // render 3D view from camera into ogre_rendertarget
    // NOTE: see https://ogrecave.github.io/ogre/api/1.11/class_ogre_1_1_render_target.html#ad0b724596d2b9e278293aee6e55a5273
    if ( ogre_rendertarget )
    {
        ogre_rendertarget->_updateViewport( ogre_camera_->getViewport(), true );
    }

    ////////////////////////////////////////////////////////////////////////////////

}


} // namespace forest

} // namespace batb



