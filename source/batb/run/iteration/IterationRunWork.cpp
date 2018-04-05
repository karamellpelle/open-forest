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
#include "batb.hpp"
#include "batb/run/iteration/IterationRunWork.hpp"
#include "batb/run/iteration/IterationRunWork/RunWorkTBWidget.hpp"
#include "batb/demo/other.hpp"

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

    // ensure Ogre is only touched in work-thread
    batb.ogre.enabled( false );
    // ensure AL is only touched in work-thread
    batb.al.enabled( false );

    // create widget
    tb_widget_ = new RunWorkTBWidget();
    batb.gui.addWidget( tb_widget_ );

    // load non-core on current context, but in background thread:
    work_.begin();


}


IterationStack IterationRunWork::iterate_run(World& world)
{
    // output 
    output( world );

    if ( auto* next = work_.current() )
    {
        if ( next != item_ ) 
        {
            //tb_widget_->set( cur->to_alpha(), cur->tag.c_str() );

            // print progress
            uint percent = (uint)( 100.0 * next->alpha );
            const char* tag = next->tag.c_str();
            std::printf( "[%3u%%] Now loading: %s\n", (unsigned int)( percent ), tag );
          
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


        // set back AL
        batb.al.enabled( true );
        // set back Ogre
        batb.ogre.enabled( true );


        // finish this iteration
        return _emptylist_; 

    }
    
}

void IterationRunWork::output(run::World& run)
{
    // draw background
    demo::background( batb, run );

    if ( item_ )
    {
        switch ( item_->mode )
        {
        case WorkItem::Mode::Definite:
        {

            //tb_widget_->set( item_->alpha, item_->tag );
            const auto& tag = item_->tag;
            auto alpha = item_->alpha;

            // draw progressbar. FIXME
            auto nvg = batb.gl.nanovg_begin( run.scene );
            float_t w = 512;
            float_t h = 22;
            float_t x = 0.5 * (run.scene.wth - w);
            float_t y = 0.5 * run.scene.hth + 112; 
            float_t r = 11;
            nvgBeginPath( nvg );
            nvgRoundedRect( nvg, x, y, w, h, r );
            nvgFillColor( nvg, nvgRGBf( 1.0, 1.0, 1.0 ) );
            nvgFill( nvg );

            nvgBeginPath( nvg );
            nvgRoundedRect( nvg, x, y, alpha * w, h, r );
            nvgFillColor( nvg, nvgRGBf( 1.0, 0.22, 0.0 ) );
            nvgFill( nvg );
            
            static int font = -1;
            if ( font == -1 )
            {
               font = batb.gl.nanovg_font( "sans", file::static_data( "batb/Ubuntu-Title.ttf" ) );
            }
            nvgFontSize( nvg, 20 );
            nvgTextAlign( nvg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE );
            
            nvgFontFaceId( nvg, font );
            nvgFillColor( nvg, nvgRGBf( 0.0, 0.0, 0.0 ) );
            //nvgFillColor( nvg, nvgRGBf( 0.11, 0.64, 0.04 ) );
            nvgText( nvg, x + 0.5 * w, y + 0.5 * h, tag.c_str(), nullptr );

            batb.gl.nanovg_end();
        }    
        break;
        case WorkItem::Mode::Indefinite:
            // FIXME: draw a finite progressbar/spinner

        break;
        }
    } 

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

