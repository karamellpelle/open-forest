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
#include "BATB/Run/Notify.hpp"
#include "BATB/Run/Console.hpp"
#include "BATB/Run/KeySet.hpp"
#include "BATB/GUI.hpp"
#include "BATB/Demo.hpp"
#include "BATB/Demo/libs/nanovg.hpp"
#include "BATB/Demo/libs/ogre.hpp"
#include "BATB/Demo/libs/al.hpp"

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

    // create main menu
    tb_main = new TBMain( batb );
    //tb_main = new TBMain();
    //tb_main->batb = batb;
    //tb_main->begin( path ); // use defined in .yaml? constexpr const char* path = "static://BATB/Run/main.tb.txt";
    // ^ TODO: tb_main

    batb->gui->addWidget( tb_main );

}

void IterationRunMain::end()
{
    // destroy the demo world
    delete demo;
    demo = nullptr;

    // remove main menu widget
    batb->gui->removeWidget( tb_main );
    delete tb_main;
    tb_main = nullptr;

}
  
////////////////////////////////////////////////////////////////////////////////
void IterationRunMain::iterate_begin(World& run)
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    // we want clean state for our Key's, no garbage:
    batb->run->keys->reset();

    // point Keys to GUI 
    batb->gui->bind( batb->keys.get() );

    // nanovg demo
    demo::nanovg::demo_begin( batb );

    // ALURE demo
    demo::al::demo_begin( batb );

    // make GUI visible
    tb_main->SetVisibility( tb::WIDGET_VISIBILITY_VISIBLE ); 

    // cool
    batb->log->endl();
    batb->log << u8"✨ M E N U ✨" << std::endl;
    {
        LogIndent indent( batb->log, u8"✅  " );
        batb->log << "run nanovg demo" << std::endl;
        batb->log << "run terrain rendering demo" << std::endl;
        batb->log << "run old (may not be built into application)" << std::endl;
        batb->log->prefix( "❌  " );
        batb->log << "exit" << std::endl;
    }
        

    // use bound controls name
    batb->log->endl();
    batb->log << "use [" << batb->run->keys->console->nameEasy() << "] to toggle console" << std::endl;
    batb->log << "use [" << batb->run->keys->fullscreen->nameEasy() << "] to toggle fullscreen" << std::endl;
    batb->log->endl();

    // notification
    std::ostringstream os; os << "use " << batb->run->keys->console->nameGUI() << " to toggle console." <<  "\n"
                              << "use " << batb->run->keys->fullscreen->nameGUI() << " to toggle fullscreen." <<  "\n";
                              //<< "fdjs lfej wijfls jasl fjeiw jfl jefaj fkl jlkajf iejlfjaghiu fghu48394 43q923 8923 98235749tujfrfsjkfle øjewf 489 u4rfdsjkl jasi 48tj gfdsER T $I$TCqTV 495 ug føre på3t4j hy";
    batb->run->notify->message( os.str(), 10.0 );

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
    //if ( batb.run.keys.escape->click() )  run.events.push( event::Do::Exit );

    // test scrool
    //double a = batb->demo->keys->scroll->alpha();
    //std::cout << "scroll! current: " << a << "\r" << std::flush;

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

                auto iterationRunDemo = batb->run->iterationRunDemo.get();

                // create demo::World 
                // FIXME: this world is never released as far as I know
                if ( demo )
                {
                    // continue with previous
                    return { game::begin_iteration( iterationRunDemo ), 
                             game::begin_iteration( this ) };
                }
                else
                {

                    // create and set up demo world
                    auto demo = newDemoWorld( &run );

                    // demo world created, make sure IterationRunDemo works on that
                    // for non-demo world, that world should be a 
                    // TODO: make demo world a subworld of run world and remove this:
                    iterationRunDemo->demoWorld( demo );

                    return { game::begin_iteration( iterationRunDemo ), 
                             game::begin_iteration( this ) };

                    // TODO: use workers to load data instead:
                    //return {
                    //          new run::IterationRunDemoBegin( demo ),
                    //          game::begin_iteration( iterationRunDemo ),
                    //          new run::IterationRunDemoEnd( demo ),
                    //          game::begin_iteration( this )
                    //       };
                }

            }    
            case event::Do::NanoVG:
            {
                // nanovg on/off
                demo::nanovg::demo_toggle();

                batb->run->notify->message( "FYI: NanoVG toggled", 5.0 );
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


// TODO: make dwmo world a subworld of run wolr and remvoe this:
demo::World* IterationRunMain::newDemoWorld(run::World* run)
{
    if ( !demo )
    {
        // create and set up demo world
        demo = new demo::World( *run );

        auto& forest = *demo->forest;

        // create a Forest world for demo
        // TODO: do something better than a loading class
        forest::WorldLoader loader( batb );        
        loader.load( forest, YAML::Node() ); 

        // now, forest world of demo world is created, continue
        // with other set ups 
        demo->begin();
    }
    return demo;

}



} // namespace run

} // namespace batb


