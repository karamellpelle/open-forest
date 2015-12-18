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
#include "batb/value/run.hpp"

namespace batb
{


namespace run
{

IterationRun::IterationRun(BATB& b) : batb( b )
{

}

IterationStack IterationRun::iterate(World& world)
{
    ////////////////////////////////////////    
    // begin new frame
    ////////////////////////////////////////
debug::gl::DebugGroup(DEBUG_FUNCTION_NAME);

    // set current tick for world.
    world.tick = batb.env.tick();

    // setup scene for this frame
    begin( world.scene );

    // step all Key's, before 'iterate_run' implementation
    batb.keys.step( world.tick );

    // setup Ogre for a new frame
    batb.ogre.frameBegin(); 

    // begin frame for AL
    batb.al.frameBegin();

    // move events from Run over to World
    // all events (from Run and subclasses of IterationRun (henc
    // there is no need for EventList's in subclasses of IterationRun)
    // are propagated down to World.
    //
    // events can not be doubled. 
    // hence only 1 run::World will receive the events from Run
    // each frame. in practice this is no problem, since we will
    // only work on 1 run::World
    //world.events.push( events_ ); 
    //
    world.events.take( batb.run.events_ );

    ////////////////////////////////////////////////////////////////////////////////
    // actual iteration, implemented by subclass
debug::gl::msg("iterate_run()");
    auto ret = iterate_run( world );
    ////////////////////////////////////////////////////////////////////////////////

    
    // end AL frame
    batb.al.frameEnd();

    // end Ogre frame
    batb.ogre.frameEnd();

    // output and step GUI (_every_ frame!)
    batb.gui.output( world.scene );
    batb.gui.step( world.tick );

    ////////////////////////////////////////////////////////////////////////////////
    // free resources

    events_step( world.events );


    ++world.frames;

    return ret;
}


// set up our Scene object for a new frame.
// currently, our Scene is just the default FBO,
// from Env
void IterationRun::begin(Scene& scene)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // set fragment size of scene.
    batb.env.screenSize( scene.wth, scene.hth );

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

