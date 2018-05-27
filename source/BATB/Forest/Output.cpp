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
#include "BATB/Forest/Output.hpp"
#include "BATB/OGRE.hpp"
#include "BATB/OGRE/helpers.hpp"
#include "OgreSingleton.h"
#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreNode.h"
#include "OgreSceneNode.h"


namespace batb
{

namespace forest
{

void Output::operator()(World& forest)
{
    run::World& run = forest.run;

  
    // TODO: instead for below, use Scene: camera->setProjection( run.scene.proj3D );
    float_t aspect = run.scene.shape.wth / run.scene.shape.hth;
    forest.camera.ogre_camera->setAspectRatio( aspect );


    ////////////////////////////////////////////////////////////////////////////////
    // set camera from its DTMovable 
    //forest.camera.ogre_scenenode->setDirection( ogre::cast_( forest.camera.move.aim[2] ) ); 
    //forest.camera.ogre_scenenode->setPosition(  ogre::cast_( forest.camera.move.aim[3] ) );

    ////////////////////////////////////////////////////////////////////////////////

    // render 3D view from camera into Scene
    // NOTE: see https://ogrecave.github.io/ogre/api/1.11/class_ogre_1_1_render_target.html#ad0b724596d2b9e278293aee6e55a5273
    batb->ogre->sceneBegin( run.scene );
    batb->ogre->outputCamera( forest.camera.ogre_camera );
    batb->ogre->sceneEnd();


    ////////////////////////////////////////////////////////////////////////////////
    // TODO: other output
    //  * sound 
    //  * network
    //  * 
}


} // namespace forest

} // namespace batb



