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
#include "batb/gui.hpp"
#include "batb/demo/libs/nanovg.hpp"
#include "batb/demo/libs/ogre.hpp"
#include "batb/demo/libs/al.hpp"
#include "batb/demo/other.hpp"
#include "batb/run/iteration/IterationRunMain/TBMain.hpp"
#include "batb/run/events.hpp"
#include "batb/run/iteration/IterationRunDemo.hpp"

//#define DEMO_FOREST_DIRECT


namespace batb
{

namespace run
{





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationRunMain::IterationRunMain(BATB& b) : IterationRun( b ), beginEvents( b )
{

}



void IterationRunMain::iterate_begin(World& run)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // we want clean state for our Key's, no garbage:
    batb.run.keyset.reset();

    // point Keys to GUI 
    batb.gui.bind( batb.keys );

    // Ogre demo
    //demo::ogre::demo_begin( batb );

    // nanovg demo
    demo::nanovg::demo_begin( batb );

    // ALURE demo
    demo::al::demo_begin( batb );

    // TODO: use TBLayout
    tb_main->SetSize( 800, 200  );
    tb_main->SetPosition( tb::TBPoint( 0, 100 ) );
    tb_main->SetVisibility( tb::WIDGET_VISIBILITY_VISIBLE ); 
    tb_main->EnsureFocus();


    std::cout << std::endl;
    std::cout << "click ESC to exit from main..." << std::endl;
    std::cout << "toggle nanovg demo with the N key..." << std::endl;
    std::cout << "toggle turbobadger demo with the T key..." << std::endl;
    std::cout << "toggle old-BATB with the INSERT key..." << std::endl; 
    std::cout << "start DemoForest with the O key..." << std::endl;
    std::cout << std::endl;


}


IterationStack IterationRunMain::iterate_run(World& run)
{
debug::gl::DebugGroup(DEBUG_FUNCTION_NAME);

    ////////////////////////////////////////////////////////////////////////////////
    //  OUTPUT
    //

    // draw background
    demo::background( batb, run );

    // nanovg demo
    if ( batb.run.keyset.nanovg->toggle() ) demo::nanovg::demo_iterate( batb, false, false );

    ////////////////////////////////////////////////////////////////////////////////
    // grab and clean up events
    beginEvents( run );
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    
    // ALURE demo:
    demo::al::demo_iterate( batb, run );

    // start forest-demo:
#ifdef DEMO_FOREST_DIRECT
    if ( true )
#else
    if ( batb.run.keyset.ogre->click() )
#endif
    {
        // remove main widget from screen
        tb_main->SetVisibility( tb::WIDGET_VISIBILITY_INVISIBLE );

        //auto* demo = new demo::World( run );
        //return { game::begin_iteration( new IterationRunWork( batb, LoadWorker<demo::World>( batb, demo ) ) ),
        //         game::begin_iteration( new IterationRunDemo( batb ) ),
        //         game::begin_iteration( new IterationRunWork( batb, UnloadWorker<demo::World>( batb, demo ) ) ),
        //         game::begin_iteration( this )
        //       }
        return { game::begin_iteration( new IterationRunDemo( batb ) ), 
                 game::begin_iteration( this ) };
    }
    

    if ( batb.run.keyset.escape->click() ) return _emptylist_;
    
    // old-BATB
    if ( batb.run.keyset.old->click() )
    {
        tb_main->SetVisibility( tb::WIDGET_VISIBILITY_INVISIBLE );

        return {  game::begin_iteration( batb.run.iterationRunOld ), 
                  game::begin_iteration( *this ) };
    }


    for ( auto& event : run.events )
    {
        ////////////////////////////////////////////////////////////////////////////////
        // event::Do
        if ( auto* next = eat<event::Do>( event ) )
        {
            switch ( *next )
            {
            case event::Do::DemoForest:
                batb.run.console( R"(echo "event: do-demo-forest")" );
                //auto* demo = new demo::World( run );
                //return { game::begin_iteration( new IterationRunWork( batb, LoadWorker<demo::World>( batb, demo ) ) ),
                //         game::begin_iteration( new IterationRunDemo( batb ) ),
                //         game::begin_iteration( new IterationRunWork( batb, UnloadWorker<demo::World>( batb, demo ) ) ),
                //         game::begin_iteration( this )
                //       }
                //return { game::begin_iteration( new IterationRunDemo( batb ) ), 
                //         game::begin_iteration( this ) };
                
            case event::Do::NanoVG:
                batb.run.console( R"(echo "do-nanovg")" );
                //run.toggle_nanovg = !run.toggle_nanovg;
                break;

            case event::Do::Old:
                batb.run.console( R"(echo "do-old")" );
                //return {  game::begin_iteration( batb.run.iterationRunOld ), 
                //          game::begin_iteration( *this ) };

            case event::Do::Exit:
                batb.run.console( R"(echo "do-exit")" );
                break;

            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        // 

    }

    // default: continue this iteration  
    return { this };
}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationRunMain& iter)
{
    BATB& batb = iter.batb;


    // set up GUI's
    // FIXME: memory leak, according to valgring
    iter.tb_main = new TBMain( batb );


    // add to screen
    // TODO: use layout
    batb.gui.addWidget( iter.tb_main );

}

void end(IterationRunMain& iter)
{
    BATB& batb = iter.batb;

    batb.gui.removeWidget( iter.tb_main );

    delete iter.tb_main;
    iter.tb_main = nullptr;
}



} // namespace run

} // namespace batb


