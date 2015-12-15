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
#include "batb/forest/Output.hpp"
#include "batb/forest.hpp"
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

/*
    ////////////////////////////////////////////////////////////////////////////////
    // turn camera automatically 
    forest.camera.ogre_camera->setPosition( Ogre::Vector3( 0, 320, 0 ) );
    float_t x,z;
    cossin( 0.25 * forest.tick, x, z );
    forest.camera.ogre_camera->setDirection( Ogre::Vector3( x, -0.18, z ) );
    ////////////////////////////////////////////////////////////////////////////////
*/

    ////////////////////////////////////////////////////////////////////////////////
    // set camera from its DTMovable 
    const auto& dir = forest.camera.move.aim[ 2 ];
    const auto& pos = forest.camera.move.aim[ 3 ];
    
    forest.camera.ogre_camera->setDirection( Ogre::Vector3( dir[0], dir[1], dir[2] ) );
    forest.camera.ogre_camera->setPosition(  Ogre::Vector3( pos[0], pos[1], pos[2] ) );

    ////////////////////////////////////////////////////////////////////////////////

    // TODO: 
    //  terrain_group->autoUpdateLodAll(false, Any( Real(HOLD_LOD_DISTANCE) )); // ???


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



