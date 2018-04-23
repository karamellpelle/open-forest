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
#include "BATB/Run/Iteration/IterationRun.hpp"
#include "BATB/Run/World.hpp"
#include "BATB/Run.hpp"
#include "BATB/Run/Notify.hpp"
#include "BATB/Run/KeySet.hpp"
#include "BATB/Run/Console.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/Keys.hpp"
#include "BATB/OGRE.hpp"
#include "BATB/AL.hpp"
#include "BATB/GL.hpp"
#include "BATB/GUI.hpp"
#include "BATB/Time.hpp"
#include "BATB/Screen.hpp"

namespace batb
{


namespace run
{

IterationRun::IterationRun(BATB* b) : batb( b )
{

}

IterationStack IterationRun::iterate(World& world)
{
    ////////////////////////////////////////    
    // begin new frame
    ////////////////////////////////////////
debug::gl::DebugGroup _dbg(DEBUG_FUNCTION_NAME);

    // set current tick for world.
    world.tick = batb->time->get();

    // setup scene for this frame
    begin( world.scene );

    // step all Key's, before 'iterate_run' implementation
    batb->keys->step( world.tick );

    // setup Ogre for a new frame
    batb->ogre->frameBegin(); 

    // begin frame for AL
    batb->al->frameBegin();

    if ( batb->run->initialized() )
    {
        // sets tick and 
        // TODO: only uses Scene
        batb->run->notify->step( world );

    }

    ////////////////////////////////////////////////////////////////////////////////
    // actual iteration, implemented by subclass
debug::gl::msg("iterate_run()");
    auto ret = iterate_run( world );
    ////////////////////////////////////////////////////////////////////////////////

    //batb.run.notifier.frameEnd(); or remove?

    
    // end AL frame
    batb->al->frameEnd();

    // end Ogre frame
    batb->ogre->frameEnd();

    // output and step GUI (_every_ frame!)
    batb->gui->output( world.scene );
    batb->gui->step( world.tick );


    if ( batb->run->initialized() )
    {
        // console always available

        if ( batb->run->keys->console->click() )
        {
            // note that 'keys.console' can not be disabled
            if ( batb->run->keys->console->toggle() ) batb->run->console->open( world ); else batb->run->console->close( world );
        }

        // update Console
        batb->run->console->step( world );

        // update Notify
        batb->run->notify->step( world );
    }

    ////////////////////////////////////////////////////////////////////////////////
    ++world.frames;

    return ret;
}


// set up our Scene object for a new frame.
// currently, our Scene is just the default FBO,
// from Env
void IterationRun::begin(Scene& scene)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    // set fragment size of scene from current screen size.
    batb->screen->getSize( scene.wth, scene.hth );

    // set shape
    scene.shape.size( scene.wth, scene.hth );

    // set 2D-projection: normalized size, x (left -> right), y (up -> down)
    scene.proj2D = glm::ortho( (float_t)( 0.0 ), scene.shape.wth, scene.shape.hth, (float_t)( 0.0f ) );

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

