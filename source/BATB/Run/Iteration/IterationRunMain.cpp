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
#include "BATB/Run.hpp"
#include "BATB/Run/Iteration.hpp"
#include "BATB/Run/Iteration/IterationRunMain/TBMain.hpp"
#include "BATB/Run/events.hpp"
#include "BATB/Run/Notifier.hpp"
#include "BATB/Run/Console.hpp"
#include "BATB/Run/KeySet.hpp"
#include "BATB/GUI.hpp"
#include "BATB/Demo/libs/nanovg.hpp"
#include "BATB/Demo/libs/ogre.hpp"
#include "BATB/Demo/libs/al.hpp"
#include "BATB/Demo/other.hpp"

//#define DEMO_FOREST_DIRECT


namespace batb
{

namespace run
{


IterationRunMain::IterationRunMain(BATB* b) : IterationRun( b ), beginEvents( b )
{

}

////////////////////////////////////////////////////////////////////////////////
//
//
void IterationRunMain::begin()
{

    tb_main = new TBMain( batb );

    // add to screen
    batb->gui->addWidget( tb_main );

}

void IterationRunMain::end()
{

    batb->gui->removeWidget( tb_main );

    delete tb_main;
    tb_main = nullptr;
}
  
////////////////////////////////////////////////////////////////////////////////
void IterationRunMain::iterate_begin(World& run)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    // we want clean state for our Key's, no garbage:
    batb->run->keyset->reset();

    // point Keys to GUI 
    batb->gui->bind( batb->keys.get() );

    // nanovg demo
    demo::nanovg::demo_begin( batb );

    // ALURE demo
    demo::al::demo_begin( batb );

    // make GUI visible
    tb_main->SetVisibility( tb::WIDGET_VISIBILITY_VISIBLE ); 

    std::cout << std::endl << "use [tab] to toggle console" << std::endl; // TODO: implement Key name

    NotifyMessage msg( "Hint: use [tab] to toggle console" );
    msg.duration = 8.0;
    batb->run->notifier->message( msg );
    // ^ here it would be nice to use convert Key to a TB widget inside text. TB has this 
    //   functionality to insert widgets in text, actually

}


IterationStack IterationRunMain::iterate_run(World& run)
{
debug::gl::DebugGroup _dbg(DEBUG_FUNCTION_NAME);

    ////////////////////////////////////////////////////////////////////////////////
    // *** output ***

    ////////////////////////////////////////////////////////////////////////////////
    // grab and clean up events (must be done between output and step!)
    beginEvents( run );
    ////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////
    // *** step ***
    

    demo::al::demo_iterate( batb, run );
    demo::nanovg::demo_iterate( batb, false, false );

    // step widget
    tb_main->step( run );

    // escape quits main (exit)
    //if ( batb.run.keyset.escape->click() )  run.events.push( event::Do::Exit );

#ifdef DEMO_FOREST_DIRECT
    run.events.push( event::Do::DemoForest );
#endif

    // think: look at events and handle thereafter
    for ( auto& event : run.events )
    {
        ////////////////////////////////////////////////////////////////////////////////
        // event::Do
        if ( auto* next = eat<event::Do>( event ) )
        {
            switch ( *next )
            {
            case event::Do::DemoForest:
            {
                // remove main widget from screen
                tb_main->SetVisibility( tb::WIDGET_VISIBILITY_INVISIBLE );

                // create demo::World 
                auto demo = new demo::World( run );
                forest::WorldLoader loader( batb );        
                loader.load( demo->forest, YAML::Node() ); 

                return { game::begin_iteration( new IterationRunDemo( batb, demo ) ), 
                         game::begin_iteration( this ) };

                //return {
                //          new run::IterationRunWork( batb, demo::LoadWorkerWorld( batb, demo ) ),
                //          game::begin_iteration( new IterationRunDemo( batb, demo ) ),
                //          new run::IterationRunWork( batb, demo::UnloadWorkerWorld( batb, demo ) ),
                //          game::begin_iteration( this )
                //       };
            }    
            case event::Do::NanoVG:
            {
                // nanovg on/off
                demo::nanovg::demo_toggle();

                // tmp
                NotifyMessage msg( "FYI: nanovg toggled." );
                msg.duration = 5.0;
                batb->run->notifier->message( msg );
                break;
            }
            case event::Do::Old:
            {
                tb_main->SetVisibility( tb::WIDGET_VISIBILITY_INVISIBLE );

                return {  game::begin_iteration( batb->run->iterationRunOld ), 
                          game::begin_iteration( *this ) };
            }
            case event::Do::Exit:
            {
                // finish this iteration, IterationRunMain
                return _emptylist_;
            }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
        // 

    }

    // default: continue this iteration  
    return { this };
}





} // namespace run

} // namespace batb


