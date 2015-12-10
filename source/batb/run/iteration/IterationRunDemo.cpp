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
#include "batb.hpp"
#include "batb/run/iteration/IterationRunDemo.hpp"

namespace batb
{

namespace run
{


IterationRunDemo::IterationRunDemo(BATB& b) : IterationRun( b )
{

}



void IterationRunDemo::iterate_begin(World& run)
{
    BATB_LOG_FUNC( batb );

    //// create stack for demo::World
    //stack_ = { new IterationDemo( batb ) };

    // load Forest
    // NOTE: this fails anyway; 'this' is destroyed upon 
    //       iteration is finished
    static bool init = false;
    if (!init)
    {
        demo = new demo::World( run );
        forest::WorldLoader loader( batb );
        loader.load( demo->forest, YAML::Node() );

        init = true;
    }

    tick_ = run.tick;
}


IterationStack IterationRunDemo::iterate_run(World& run)
{
/*
    // TODO: demo::iterate_terrain
    // iterate demo::World
    game::iterate_stack( stack_, world_ )
    if ( stack_.empty() )
    {
        return _;
    }
    else
    {
        return { this };
    }
*/   
/*
    constexpr tick_t time = 4.0;
    if ( tick_ + time <= world.tick )
    {
        batb.log << "out of IterationRunDemo!!" << std::endl;
        return _;
    }
    else
    {
        return { this };
    }
*/
    float_t aspect = run.scene.shape.wth / run.scene.shape.hth;

    batb.ogre.sceneBegin( run.scene );
    demo->forest.camera.ogre_camera->setAspectRatio( aspect );
    // only render the scenemanager for this forest::World
    batb.ogre.outputCamera( demo->forest.camera.ogre_camera );
    batb.ogre.sceneEnd();

    // TODO: camera->setProjection( run.scene.proj3D );
    //terrain_group->autoUpdateLodAll(false, Any( Real(HOLD_LOD_DISTANCE) ));
    //
    /*
    tick_t tick = run.tick;
    float_t x,z;
    cossin( 0.1 * tick, x, z );
    float_t y = sin( tick * 3 );

    //Vector3 pos( 0, 400, 0 );
    //camera->setPosition( pos );
    Ogre::Vector3 dir( x, -0.14, z );
    dir.normalise();
    //camera->setDirection( dir );
    */
    /*
    if ( forest.runners.empty() )
    {
        std::cout << "runners.empty!!\n"; 
        camera->setDirection( dir );
    }
    else
    {
        forest::Runner runner = forest.runners.front();
        glm::mat4 aim = runner.aim;
        glm::vec4 z = aim[2];
        
        camera->setDirection( Ogre::Vector3( z[0], z[1], z[2] ) );

        glm::vec4 pos = runner.pos;
        camera->setPosition( Ogre::Vector3( pos[0], pos[1], pos[2] ) );

    }
    */ 


    if ( batb.run.keyset.ogre->click() )
    {
        batb.log << "out of IterationRunDemo!!" << std::endl;
        return _;
    }
    else
    {
        return { this };
    }
}
////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb

