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
#include "BATB/Demo.hpp"
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
    batb->log << "" << std::endl;
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
    std::cout << std::endl << "use [" << batb->run->keys->console->nameEasy() << "] to to toggle console" << std::endl;

    // use bound controls widget representing the control
    std::ostringstream os; os << "use " << batb->run->keys->console->nameGUI() << " to to toggle console";
    NotifyMessage msg( os.str() );
    msg.duration = 8.0;
    batb->run->notifier->message( msg );

     
     //, createKeyClicker(Key* k),
     //createKeyAlpha(Key* k),
     //createKeyPointer(Key* x, Key* y, Key* l, Key* r),
     //createKeyPointer(),
    auto k0 = (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_SLASH) );
    auto k1 = (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_COMMA) );
    auto k2 = (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_ENTER) );
    auto k3 = (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_RIGHT) );
    auto k13= (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_LEFT) );
    auto k4 = (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_SPACE) );
    auto k5 = (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_L) );
    auto k11 = (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_KP_DECIMAL) );
    auto k12= (keys::Key*)( batb->keys->createKeyButton(GLFW_KEY_KP_MULTIPLY) );
    auto k6 = (keys::Key*)( batb->keys->createKeyMouseButton(GLFW_MOUSE_BUTTON_LEFT) );
    auto k7 = (keys::Key*)( batb->keys->createKeyMouseAxisX() );
    auto k8 = (keys::Key*)( batb->keys->createKeyMouseAxisY() );
    auto k9 = (keys::Key*)( batb->keys->createKeyMouseScrollX() );
    auto k10= (keys::Key*)( batb->keys->createKeyMouseScrollY() );
keys::Key* ks[] = {k0 ,
k1 ,
k2 ,
k3 ,
k13,
k4 ,
k5 ,
k11,
k12,
k6 ,
k7 ,
k8 ,
k9 ,
k10 };
    size_t len = sizeof( ks ) / sizeof( keys::Key* );
    for (size_t i = 0; i != len; ++i)
    {
        auto k = ks[i];
        std::ostringstream os; os << "use " << k->nameGUI() << " to to toggle console";
        NotifyMessage msg( os.str() );
        msg.duration = 10.0 + i;
        batb->run->notifier->message( msg );
    }
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
    double a = batb->demo->keys->scroll->alpha();
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


