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
#include "BATB/Run/events.hpp"
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

IterationStack IterationRun::iterate(World& run)
{
debug::gl::DebugGroup _dbg(DEBUG_FUNCTION_NAME);

    ////////////////////////////////////////////////////////////////////////////////
    // begin new framw
    //

    // free old events
    run.events.step();
    
    // set current tick for world (the tick of this frame)
    run.tick = batb->time->get();

    ////////////////////////////////////////////////////////////////////////////////
    // various stepping, before 'iterate_run'. this is done before the subclass' 
    // 'iteration_run()' because
    //

    batb->keys->step( run.tick );

    if ( batb->run->initialized() )
    {
        // console always available

        if ( batb->run->keys->console->click() )
        {
            // note that 'keys.console' can not be disabled
            //FIXME if ( batb->run->keys->console->toggle() ) batb->run->console->open( run ); else batb->run->console->close( run );
        }

        // update Console
        //FIXME batb->run->console->step( run );

        // update Notify
        //FIXME batb->run->notify->step( run );

        ////////////////////////////////////////////////////////////////

        // are we going fullscreen or nonfullscreen?
        if ( batb->run->keys->fullscreen->click() )
        {
            bool full = batb->run->keys->fullscreen->toggle(); 
            batb->screen->fullscreen( full );
        }
    }

    // signalize if screen wants to quit
    if ( batb->screen->closing() )
    {
        run.events.push( event::Do::Exit );
        // and clear close flag of screen to prevent events every frame
        // however, it doesn't matter because events have a short lifetime
        batb->screen->closingClear();
    }

    // move all events from run::Run over to run::World. this is not really
    // correct behaviour - events should be copied - because Run (and IterationRun's)
    // works on any run::World. however, events can not be doubled (yet). (that's 
    // because EventList::step() decreases the Event's frame lifes
    //
    // in practice this is no since we will only work on 1 run::World each frame
    run.events.take( *batb->run->events );

    ////////////////////////////////////////////////////////////////////////////////
    // setup output frame

    // setup scene for this frame
    begin( run.scene );

    // setup Ogre for a new frame
    batb->ogre->frameBegin();

    // begin frame for AL
    //FIXME batb->al->frameBegin();

    ////////////////////////////////////////////////////////////////////////////////
    // iterate run::World - our application world
    // actual behaviour defined by subclass of this
debug::gl::msg("iterate_run()");
    auto ret = iterate_run( run );


    ////////////////////////////////////////////////////////////////////////////////
    // end output frame
    
    // end AL frame
    //FIXME batb->al->frameEnd();

    // end Ogre frame
    batb->ogre->frameEnd();

    // output and step GUI (_every_ frame!)
    //FIXME batb->gui->output( run.scene );
    //FIXME batb->gui->step( run.tick );


    ////////////////////////////////////////////////////////////////////////////////
    // end frame
    //

    ++run.frames;

    // subclass defines next iteration top (thinks)
    return ret;
}


// set up scene for this frame
// TODO: use the Scene's FBO
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

    // set framebuffer to our scene's FBO. TODO
    //glBindFramebuffer( gl_FRAMEBUFFER, scene.fbo );

    // clear screen for the new frame
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

}


} // namespace run

} // namespace batb

