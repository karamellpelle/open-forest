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
#include "batb/run/iteration/IterationRunMain/RunMainTBWidget.hpp"
#include "batb/run/events.hpp"


namespace batb
{

namespace run
{


void eat_a(const EventA& a)
{
    std::cout << "eating event EventA: " << a.name <<  std::endl;
}
void eat_b(const EventB& b)
{
    std::cout << "eating event EventB: " << b.x << "*" << b.y << std::endl;
}
void eat_uint(const uint& n)
{
    std::cout << "eating event uint: " << n << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationRunMain::IterationRunMain(BATB& b) : IterationRun( b )
{

    eater_set.push( eat_a );
    eater_set.push( eat_b );
    eater_set.push( eat_uint );
    eater_set.push( &IterationRunMain::eat_number, this );
    //EventEater eater(  &IterationRunMain::eat_number, *this );
}



void IterationRunMain::iterate_begin(World& run)
{
    BATB_LOG_FUNC( batb );

debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // we want clean state for our Key's, no garbage:
    batb.run.keyset.reset();

    // point Keys to GUI 
    batb.gui.bind( batb.keys );


    if ( tb_widget_->GetParent() == nullptr )
    {
        batb.gui.addWidget( tb_widget_ );
    }

    // FIXME: demo_end()!
    //
    // Ogre demo
    demo::ogre::demo_begin( batb );

    // nanovg demo
    demo::nanovg::demo_begin( batb );

    // ALURE demo
    demo::al::demo_begin( batb );

    std::cout << std::endl;
    std::cout << "click ESC to exit from main..." << std::endl;
    std::cout << "toggle Ogre demo with the O key..." << std::endl;
    std::cout << "toggle nanovg demo with the N key..." << std::endl;
    std::cout << "toggle turbobadger demo with the T key..." << std::endl;
    std::cout << "toggle old-BATB with the INSERT key..." << std::endl; 
    std::cout << std::endl;


    // tmp:
    forest = new forest::World( run ); // FIXME: delete!
    forest::Runner runner;
    forest->runners.push_front( runner );

    forest_stack = 
    {
        game::begin_iteration( batb.forest.iterationForestDemo )
    };
    
}


IterationStack IterationRunMain::iterate_run(World& run)
{
    ////////////////////////////////////////////////////////////////////////////////
    //  OUTPUT
    //
debug::gl::DebugGroup(DEBUG_FUNCTION_NAME);

    // add events from this to world
    run.events.move( events_ );

    if ( batb.run.keyset.u->click() ) run.toggle_a = !run.toggle_a;
    if ( batb.run.keyset.ogre->click() ) run.toggle_ogre = !run.toggle_ogre;
    if ( batb.run.keyset.nanovg->click() ) run.toggle_nanovg = !run.toggle_nanovg;
    if ( batb.run.keyset.tb->click() ) run.toggle_tb = !run.toggle_tb;


    // tmp:
    eater_set.eat( run.events );
    if ( batb.run.keyset.i->click() )
    {
        run.toggle_b = !run.toggle_b;
        std::cout << "FPS: " << batb.env.frameFPS() << std::endl;
        // push event
        static uint ix = 0;
        if ( ix == 0 ) 
        {
            std::ostringstream os;
            os << "fps: " << batb.env.frameFPS();
            run.events.push( EventA( os.str() ) );
        }
        if ( ix == 1 )
        {
            uint x, y;
            batb.keys.getCursorPos( x, y );
            run.events.push( new EventB( x, y ) );
        }
        if ( ix == 2 )
        {
            static uint m = 0;
            run.events.push( m++ );
        }
        ix = (ix + 1) % 3;
    }

    // Ogre demo:
    if ( run.toggle_ogre )
    {
        demo::ogre::demo_iterate( batb, run, *forest );
    }
    // nanovg demo:
    if ( run.toggle_nanovg )
    {
        demo::nanovg::demo_iterate( batb, false, false );
    }
    

    // forest
    game::iterate( forest_stack, *forest );

    // ALURE demo:
    demo::al::demo_iterate( batb, run );

    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    // 
    
    

    if ( batb.run.keyset.pause->click() )
    {
        // escaping from IterationMain finishes iteration.
        batb.log << "IterationRunMain ->" << std::endl;
        return _;
    }

    // run old-BATB if old-key released
    if ( batb.run.keyset.old->released() )
    {
        // remove window
        //if ( widget_->GetParent() != nullptr )
        if ( tb_widget_->GetParent() != nullptr )
        {
            batb.gui.removeWidget( tb_widget_ );
        }

        batb.log << "IterationRunMain -> IterationRunOld" << std::endl;
        return {  game::begin_iteration( batb.run.iterationRunOld ), 
                  game::begin_iteration( *this ) };
    }

    IterationStack nexts = { this };

/*
    // look at events
    caseOf( run.events )
    (
        [&](const DoDemo& demo)
        {
            //if ( demo == DoDemo::Forest ) nexts = { new IterationRunDemoForest(), this };
            //if ( demo == DoDemo::XXX ) nexts = { new IterationRunDemoForest(), this };
        },
        [&](const uint& value)
        {
            batb.log << "run::World: event value " << value << std::endl;
        }
    );
*/
    return nexts;
}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationRunMain& iter)
{
    BATB_LOG_FUNC( iter.batb );
/*
    // create GUI widget. GUI is up and running...
    // NOTE: memory management is performed by TB! NO!!:
    // FIXME: memory leak, according to valgring
    auto* window = new tb::TBWindow(); // mem-leak
    window->SetSettings( tb::WINDOW_SETTINGS_TITLEBAR | tb::WINDOW_SETTINGS_RESIZABLE | tb::WINDOW_SETTINGS_CAN_ACTIVATE );
    window->SetSize(250, 630 );
    //window->Position( 40, 40 );

    window->SetText( "testing turbobadger. a window." );
    window->SetGravity( tb::WIDGET_GRAVITY_ALL );
    iter.widget_ = window;
*/
    // set up GUI's
    // FIXME: memory leak, according to valgring
    iter.tb_widget_ = new RunMainTBWidget( iter );


}

void end(IterationRunMain& iter)
{
    BATB_LOG_FUNC( iter.batb );

    iter.batb.gui.removeWidget( iter.tb_widget_ );
    delete iter.tb_widget_;
    iter.tb_widget_ = nullptr;
}



} // namespace run

} // namespace batb


