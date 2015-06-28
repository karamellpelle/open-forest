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
#include "batb.hpp"
#include "batb/run/iteration/IterationRun.hpp"
#include "batb/run/World.hpp"

namespace batb
{


namespace run
{

IterationRun::IterationRun(BATB& b) : batb( b )
{

}

void IterationRun::iterate(IterationStack& stack, World& world)
{
    ////////////////////////////////////////    
    // begin new frame
    ////////////////////////////////////////
debug::gl::DebugGroup(DEBUG_FUNCTION_NAME);

    // set current world the Run-object is working on
    batb.run.world = &world;

    // tmp:
    if ( world.toggle_a )
        glClearColor( 0.4, 0.1, 0.5, 1.0 );
    else
        glClearColor( 0.0, 0.0, 0.0, 0.0 );

    // setup scene for this frame
    begin( world.scene );

    // draw OGRE 
    // FIXME: out of the general IterationRun and into specific 
    //        IterationRunXXX? Is there only 1 Ogre, or can we
    //        use more than one Ogre::SceneManager's?
    //        
    // tmp:
    if (world.toggle_ogre)
    {
        batb.ogre.output( world.scene );
    }

    ////////////////////////////////////////
    // actual iteration, implemented by subclass
    ////////////////////////////////////////
debug::gl::msg("iterate_run()");
    iterate_run( stack, world );
 

    // set current tick for world.
    // TODO: not here, done by wrapped iteration ??
    world.tick = env::tick();

    // tmp:
    if ( world.toggle_tb )
    {
        batb.gui.output( world.scene );
        batb.gui.step( world.tick );
    }

    // update keys
    batb.run.keyset.step( world.tick );


    // TODO: finish up, free mem (events, ...)


    // count number of IterationRun-iterations
    ++world.frames_count;

}


// set up our Scene object for a new frame.
// currently, our Scene is just the default FBO,
// the FBO for our env::screen.
void IterationRun::begin(Scene& scene)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // set fragment size of scene. FBO is env::screen
    env::screen_size( scene.wth, scene.hth );

    // set shape
    scene.shape.size( scene.wth, scene.hth );

    // set 2D-projection: normalized size, x (left -> right), y (up -> down)
    scene.proj2D = glm::ortho( 0.0, scene.shape.wth, scene.shape.hth, 0.0 );

    // set 3D-projection: x ( ), y ( ), z ( ) . FIXME.
    scene.proj3D = glm::perspective( value::proj3DFOVY, scene.shape.wth / scene.shape.hth, value::proj3DNear, value::proj3DFar );


    ////////////////////////////////////////    
    // OpenGL
    ////////////////////////////////////////

    // define fragment region
    glViewport( 0, 0, scene.wth, scene.hth );

    // bind FBO 
    //glBindFramebuffer( gl_FRAMEBUFFER, scene.fbo );

    // clear screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

}


} // namespace run

} // namespace batb

