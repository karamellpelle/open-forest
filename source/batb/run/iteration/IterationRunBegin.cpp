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
#include "batb/run/iteration/IterationRunBegin.hpp"
#include "batb/run/iteration/IterationRunBegin/RunBeginTBWidget.hpp"

namespace batb
{

namespace run
{


IterationRunBegin::IterationRunBegin(BATB& b) : IterationRun( b ), 
                                                loader_( b )
{

}


void IterationRunBegin::iterate_begin(World& world)
{
    BATB_LOG_FUNC( batb );

    ////////////////////////////////////////////////////////////////////////////////
    // NOTE:
    // the invariant that this thread only touches OpenGL during IO, and the loading
    // thread use IO fully, but use a different OpenGL context. also, these two threads
    // must work on disjoint memory as usual, but this should not be a problem, since
    // this thread is not interrested in BATB/run::World.

    batb.gui.addWidget( tb_widget_ );

    // load non-core on current context, but in background thread:
    loader_.begin();


}


IterationStack IterationRunBegin::iterate_run(World& world)
{

    tmp_output();


    if ( FiniteLoad* cur = loader_.current() )
    {
        static FiniteLoad* current = nullptr;

        if ( cur != current ) 
        {
            tb_widget_->set( cur->to_alpha(), cur->tag.c_str() );
/*
            uint percent = (uint)( 100.0 * cur->to_alpha() );
            const char* tag = cur->tag.c_str();
            std::printf( "[%3u%%] Now loading: %s\n", percent, tag );
*/
        }

        current = cur;

        return { this };
    }
    else
    {

        // end background loading in thread
        loader_.end();

        ////////////////////////////////////////////////////////////////////////////////
        // we now succeded to load all parts of BATB (core and non-core),
        //
        // clean up and start next iteration

        tb_widget_->set( 1.0 );

        // TODO: delete tb_widget_ and set to nullptr to save memory?
        batb.gui.removeWidget( tb_widget_ ); // FIXME
        //tb_widget_->Die();

        // let IterationRunMain start the actual game
        // TODO: IterationRunIntro first
        return { game::begin_iteration( batb.run.iterationRunMain ) }; // FIXME: mem leak according to valgrind (IterationBegin)

    }
    
    
}

void IterationRunBegin::tmp_output()
{

    // output
    gl::begin_nanovg();

        float t = 1.0 * batb.env.tickNow();
        float a = (float)( t - (uint)( t ) );
    int winWidth, winHeight;
    glfwGetWindowSize( batb.env.window, &winWidth, &winHeight );
    int fbWidth, fbHeight;
    glfwGetFramebufferSize( batb.env.window, &fbWidth, &fbHeight );

    auto ctx = batb.gl.nvg_context;

    float pxRatio = (float)fbWidth / (float)winWidth; // Calculate pixel ration for hi-dpi devices.
    nvgBeginFrame( ctx, winWidth, winHeight, pxRatio);

    //nvgSave( ctx );
    //nvgScale( ctx, (float)(forest.run.scene.wth), (float)(forest.run.scene.hth) );

    // draw pulsating square
    nvgStrokeWidth( ctx, 32 );
    nvgLineCap(ctx, NVG_ROUND );
    nvgStrokeColor(ctx, nvgRGBA(100,192,0,128));
    nvgBeginPath(ctx);
    float r = 64.0 + a * 32.0;
    float x = fbWidth * 0.5;
    float y = fbHeight * 0.5;
	nvgRoundedRect(ctx, x - r, y - r, 2 * r, 2 * r, 0.5 * r );
    nvgStroke(ctx);
   
    
    //nvgRestore( ctx );
    nvgEndFrame( ctx );

    gl::end_nanovg();


}

void IterationRunBegin::Loader::run()
{

    // setup context to our invariant
    gl::init_state();

    // NOTE: number must be updated to correct number of loads
    FiniteLoad loading( 4 + 3 );

    
    try
    {

        // load gOGRE
        ////////////////////////////////////////////////////////////////////////////////
        // NOTE: when loading Ogre in different GL context, the Terrain component becomes
        //       diffused and cause some minor rendering artifacts when changing back
        //       to main context. 
        push_current( loading( "OGRE" ) );
        ogre::begin( batb.ogre );
        ++loading;

        // load AL
        push_current( loading( "AL" ) );
        al::begin( batb.al );
        ++loading;

        // load the non-core part of run
        push_current( loading( "Run" ) );
        run::begin( batb.run );
        ++loading;

        // load race
        //push_current( loading( "" ) );
        //race::begin( batb.race );
        //++loading;

        // load forest
        push_current( loading( "Forest" ) );
        forest::begin( batb.forest );
        ++loading;


        // tmp: fake loading, to show capabilities:
        push_current( loading( "Proxy library A" ) );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        ++loading;
        push_current( loading( "Proxy library B" ) );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        ++loading;
        push_current( loading( "Proxy library C" ) );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        ++loading;

    }
    catch (std::exception& e)
    {
        batb.log << "IterationRunBegin::Loader: error: " << e.what() << std::endl; 
    }

    // must be done to signalize completion
    push_current( nullptr );
}

////////////////////////////////////////////////////////////////////////////////
//

void begin(IterationRunBegin& iter)
{
    BATB_LOG_FUNC( iter.batb );

    // create widget
    iter.tb_widget_ = new RunBeginTBWidget();
}

void end(IterationRunBegin& iter)
{
    BATB_LOG_FUNC( iter.batb );
    
    // FIXME:
    //iter.batb.gui.removeWidget( iter.tb_widget_ );
    //delete iter.tb_widget_;
    //iter.tb_widget_ = nullptr;
}


} // namespace run

} // namespace batb

