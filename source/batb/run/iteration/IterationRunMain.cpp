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
#include "batb/run/iteration/IterationRunMain/RunMainTBWidget.hpp"
#include "batb/run/events.hpp"
#include "batb/run/iteration/IterationRunDemo.hpp"

#define DEMO_FOREST_DIRECT

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


IterationRunMain::IterationRunMain(BATB& b) : IterationRun( b ), beginEvents( b )
{

    // set event eaters for received events
    event_eat.push( eat_a );
    event_eat.push( eat_b );
    event_eat.push( eat_uint );
    event_eat.push( &IterationRunMain::eat_number, this );
}



void IterationRunMain::iterate_begin(World& run)
{
    BATB_LOG_FUNC( batb );

debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // we want clean state for our Key's, no garbage:
    batb.run.keyset.reset();

    // point Keys to GUI 
    batb.gui.bind( batb.keys );

    // add main widget
    if ( tb_widget_->GetParent() == nullptr )
    {
        batb.gui.addWidget( tb_widget_ );
    }
    tb_widget_->SetVisibilility( tb::WIDGET_VISIBILITY_VISIBLE ); // lililili :)

    // Ogre demo
    //demo::ogre::demo_begin( batb );

    // nanovg demo
    demo::nanovg::demo_begin( batb );

    // ALURE demo
    demo::al::demo_begin( batb );

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
    ////////////////////////////////////////////////////////////////////////////////
    //  OUTPUT
    //
    demo::background( batb, run );

debug::gl::DebugGroup(DEBUG_FUNCTION_NAME);
    if ( run.toggle_nanovg )
    {
        demo::nanovg::demo_iterate( batb, false, false );
    }

    ////////////////////////////////////////////////////////////////////////////////
    // grab and clean up events
    beginEvents( run );
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    // 
    
    if ( batb.run.keyset.u->click() ) run.toggle_a = !run.toggle_a;
    if ( batb.run.keyset.i->click() ) run.toggle_b = !run.toggle_b;
    if ( batb.run.keyset.ogre->click() ) run.toggle_ogre = !run.toggle_ogre;
    if ( batb.run.keyset.nanovg->click() ) run.toggle_nanovg = !run.toggle_nanovg;
    if ( batb.run.keyset.tb->click() ) run.toggle_tb = !run.toggle_tb;

    // print frames per second
    if ( batb.run.keyset.u->click() ) std::cout << "fps: " << batb.env.frameFPS() << std::endl;

    // push event on click i
    if ( batb.run.keyset.i->click() )
    {
        // push event
        static uint ix = 0;
        if ( ix == 0 ) 
        {
            std::ostringstream os;
            os << "fps: " << batb.env.frameFPS();
            batb.run.pushEvent( EventA( os.str() ) );
        }
        if ( ix == 1 )
        {
            uint x, y;
            batb.keys.getCursorPos( x, y );
            batb.run.pushEvent( new EventB( x, y ) );
        }
        if ( ix == 2 )
        {
            static uint m = 0;
            batb.run.pushEvent( m++ );
        }
        ix = (ix + 1) % 3;
    }

    // ALURE demo:
    demo::al::demo_iterate( batb, run );

    // start ogre-demo:
#ifdef DEMO_FOREST_DIRECT
    if ( true )
#else
    if ( batb.run.keyset.ogre->click() )
#endif
    {
        // remove main widget from screen
        tb_widget_->SetVisibilility( tb::WIDGET_VISIBILITY_INVISIBLE );

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
    
    // run old-BATB if old-key released
    if ( batb.run.keyset.old->released() )
    {
        // remove widget from screen
        if ( tb_widget_->GetParent() != nullptr )
        {
            batb.gui.removeWidget( tb_widget_ );
        }

        //batb.log << "IterationRunMain -> IterationRunOld" << std::endl;
        return {  game::begin_iteration( batb.run.iterationRunOld ), 
                  game::begin_iteration( *this ) };
    }


    for ( auto& event : run.events )
    {
        // general eat function
        event_eat( event );

        ////////////////////////////////////////////////////////////////////////////////
        // uint
        if ( auto* n = eat<uint>( event ) )
        {
            std::cout << "a special event occured: " << *n << std::endl;

        }
      
        ////////////////////////////////////////////////////////////////////////////////
        // event::DoDemo
        if ( auto* next = eat<event::DoDemo>( event ) )
        {
            switch ( *next )
            {
            case event::DoDemo::Forest:
                batb.run.console( R"(echo "do forest")" );
                //auto* demo = new demo::World( run );
                //return { game::begin_iteration( new IterationRunWork( batb, LoadWorker<demo::World>( batb, demo ) ) ),
                //         game::begin_iteration( new IterationRunDemo( batb ) ),
                //         game::begin_iteration( new IterationRunWork( batb, UnloadWorker<demo::World>( batb, demo ) ) ),
                //         game::begin_iteration( this )
                //       }
                //return { game::begin_iteration( new IterationRunDemo( batb ) ), 
                //         game::begin_iteration( this ) };
                
            case event::DoDemo::Turbobadger:
                batb.run.console( R"(echo "do turbobadger")" );
                break;
            case event::DoDemo::NanoVG:
                batb.run.console( R"(echo "do nanovg")" );
                //run.toggle_nanovg = !run.toggle_nanovg;
                break;
            case event::DoDemo::Old:
                batb.run.console( R"(echo "do old")" );
                //return {  game::begin_iteration( batb.run.iterationRunOld ), 
                //          game::begin_iteration( *this ) };
            

            }

            ////////////////////////////////////////////////////////////////////////////////
            //
        }

    }

    // default: continue this iteration  
    return { this };
}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationRunMain& iter)
{
    BATB_LOG_FUNC( iter.batb );

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


