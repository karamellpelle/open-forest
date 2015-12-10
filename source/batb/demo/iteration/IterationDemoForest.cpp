//    open-demo: an orientering game.
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
#include "batb.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <random>
#include "helpers/bezier.hpp"

namespace batb
{

namespace demo
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationDemoForest::IterationDemoForest(BATB& b) : 
    IterationDemo( b ), modifyControlCamera( b ), modifyControlRunner( b ), stepDT( b )
{

}



void IterationDemoForest::iterate_begin(World& demo)
{
    BATB_LOG_FUNC( batb );

}


IterationStack IterationDemoForest::iterate_demo(World& demo)
{
/*
 *  from demo_iterate
    float_t aspect = run.scene.shape.wth / run.scene.shape.hth;

    camera->setAspectRatio( aspect );
    // TODO: camera->setProjection( run.scene.proj3D );

    //iterate_head(batb, run);
    iterate_terrain(batb, run, forest);

    camera->setAspectRatio( aspect );
*/ 

// from demo::iterate_terrain
/*
    //terrain_group->autoUpdateLodAll(false, Any( Real(HOLD_LOD_DISTANCE) ));
    tick_t tick = run.tick;
    float_t x,z;
    cossin( 0.1 * tick, x, z );
    float_t y = sin( tick * 3 );

    //Vector3 pos( 0, 400, 0 );
    //camera->setPosition( pos );
    Ogre::Vector3 dir( x, -0.14, z );
    dir.normalise();
    //camera->setDirection( dir );
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
/*
 *  from IterationForestDemo:
    batb.forest.modifyBegin( world );

    tick_t tick = world.run.tick;

    while ( world.tick + value::forestDT <= tick )
    {
        batb.forest.stepDT( value::forestDT, world );
        world.tick += value::forestDT;

        // look at events:
        // if xxx return _;
    }
*/

    // control Camera
    modifyControlCamera( world );

    // controlRunner
    modifyControlRunner( world );

    constexpr tick_t dt = 0.02;
    tick_t tick_next = forest.run.tick;

    while ( forest.tick + dt <= tick_next )
    {
      
        // step World
        stepDT( forest, dt );

        forest.tick += dt;
    }

    // TODO: look at events! and purge

    // continue with this iteration
    return { this };

}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationDemoForest& iter)
{
    BATB_LOG_FUNC( iter.batb );


}

void end(IterationDemoForest& iter)
{
    BATB_LOG_FUNC( iter.batb );
}

////////////////////////////////////////////////////////////////////////////////
// 
//



} // namespace demo

} // namespace batb

