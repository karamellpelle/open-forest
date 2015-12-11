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
#include "batb/run/iteration/IterationRunWork.hpp"
#include "batb/run/iteration/IterationRunWork/RunWorkTBWidget.hpp"

namespace batb
{

namespace run
{


IterationRunWork::IterationRunWork(BATB& b) : IterationRun( b ), work_( b )
{
    
}

IterationRunWork::~IterationRunWork()
{
    delete tb_widget_;
}

void IterationRunWork::work(std::function<void(Work& )> f)
{
    work_.work( f );

}
void IterationRunWork::iterate_begin(World& world)
{
    BATB_LOG_FUNC( batb );

    ////////////////////////////////////////////////////////////////////////////////
    // NOTE:
    // the invariant that this thread only touches OpenGL during IO, and the loading
    // thread use IO fully, but use a different OpenGL context. also, these two threads
    // must work on disjoint memory as usual, but this should not be a problem, since
    // this thread is not interrested in BATB/run::World.

    // create widget
    tb_widget_ = new RunWorkTBWidget();
    batb.gui.addWidget( tb_widget_ );

    // load non-core on current context, but in background thread:
    work_.begin();


}


IterationStack IterationRunWork::iterate_run(World& world)
{
    // output 
    output( item_ );

    if ( auto* next = work_.current() )
    {
        if ( next != item_ ) 
        {
            //tb_widget_->set( cur->to_alpha(), cur->tag.c_str() );

            // print progress
            uint percent = (uint)( 100.0 * next->alpha );
            const char* tag = next->tag.c_str();
            std::printf( "[%3u%%] Now loading: %s\n", percent, tag );
          
            item_ = next;
        }

        // continue work
        return { this };
    }
    else
    {
        ////////////////////////////////////////////////////////////////////////////////
        // work is now done.
        //

        // clean up and start next iteration
        // end background loading in thread
        work_.end();

        // tmp:
        tb_widget_->set( 1.0 );

        // TODO: delete tb_widget_ and set to nullptr to save memory?
        batb.gui.removeWidget( tb_widget_ ); // FIXME
        //tb_widget_->Die();

        // finish this iteration
        return _; 
    }
    
}

void IterationRunWork::output(WorkItem* item)
{
    if ( item )
    {
        switch ( item->mode )
        {
        case WorkItem::Mode::Definite:
            // FIXME: draw a finite progressbar
            tb_widget_->set( item->alpha, item->tag );

        break;
        case WorkItem::Mode::Indefinite:
            // FIXME: draw a finite progressbar/spinner

        break;
        }
    } 

    // tmp output
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
    nvgStrokeColor(ctx, nvgRGBA(100,0,192,128));
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


void Work::run()
{
    // do work in implementation
    function_( *this );
}


// number of work steps are known.
// it is still possible to change to indefinite 
void Work::definite(uint ns)
{
      ns_ = ns;
      mode_ = WorkItem::Mode::Definite;
};

// number of work steps are not known.
// it is still possible to change to definite 
void Work::indefinite()
{
      mode_ = WorkItem::Mode::Indefinite;
};

void Work::state(const std::string& tag, float_t alpha)
{
    WorkItem item;
    item.mode = mode_;
    item.tag = tag;
    item.alpha = alpha;
    //item.tick = batb.env.tick(); // TODO: use world tick!!

    push_current( &item );
}

void Work::state(const std::string& tag)
{
    state( tag, (float_t)( n_ ) / (float_t)( ns_ ) );
    ++n_;
}

// update (for Indefinite)
void Work::state()
{
    // TODO
    // prevent too many pushes
    //if ( tick_ + limit <= now ) { push_state( tag_ ); } 
}

void Work::finish()
{
    push_current( nullptr );
}

////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb

