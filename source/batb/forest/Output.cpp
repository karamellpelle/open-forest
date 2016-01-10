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
#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreNode.h"
#include "batb/forest/Output.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb/ogre/helpers.hpp"
#include "batb.hpp"


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
    forest.camera.ogre_camera->setDirection( ogre::cast_( forest.camera.move.aim[2] ) ); 
    forest.camera.ogre_camera->setPosition(  ogre::cast_( forest.camera.move.aim[3] ) );

    ////////////////////////////////////////////////////////////////////////////////

    // TODO: terrain_group->autoUpdateLodAll(false, Any( Real(HOLD_LOD_DISTANCE) )) ?

    // render 3D view from camera into Scene
    batb.ogre.sceneBegin( run.scene );
    batb.ogre.outputCamera( forest.camera.ogre_camera );
    batb.ogre.sceneEnd();


    ////////////////////////////////////////////////////////////////////////////////
    // TODO: other output
    //  * sound 
    //  * network
    //  * 
}


} // namespace forest

} // namespace batb



