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
#include "batb/tmp/nanovg.hpp"
#include "batb/tmp/ogre.hpp"
#include "batb/tmp/al.hpp"


namespace batb
{

namespace run
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationRunMain::IterationRunMain(BATB& b) : IterationRun( b )
{

}



void IterationRunMain::iterate_begin(World& run)
{
    BATB_LOG_FUNC( batb );

debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // we want clean state for our Key's, no garbage:
    batb.run.keyset.reset();

    // let GUI grab the Keys callbacks
    batb.gui.bind( batb.keys );


    // show GUI window
    //if ( widget_->GetParent() == nullptr )
    if ( batb.run.guiMain->GetParent() == nullptr )
    {
        //batb.gui.root.AddChild( widget_ );
        batb.gui.root.AddChild( batb.run.guiMain );
    }

    // Ogre demo
    tmp::ogre::demo_begin( batb );

    // nanovg demo
    tmp::nanovg::demo_begin();

    // ALURE demo
    tmp::al::demo_begin( batb );

    std::cout << std::endl;
    std::cout << "click ESC to exit from main..." << std::endl;
    std::cout << "toggle Ogre demo with the O key..." << std::endl;
    std::cout << "toggle nanovg demo with the N key..." << std::endl;
    std::cout << "toggle turbobadger demo with the T key..." << std::endl;
    std::cout << "toggle old-BATB with the INSERT key..." << std::endl; 
    std::cout << std::endl;


    // tmp:
    forest = new forest::World( run );
    forest::Runner runner;
    forest->runners.push_front( runner );

    forest_stack.next
    (
        game::begin_iteration( batb.forest.iterationForestDemo )
    );
    
}


void IterationRunMain::iterate_run(IterationStack& stack, World& run)
{
    ////////////////////////////////////////////////////////////////////////////////
    //  OUTPUT
    //
debug::gl::DebugGroup(DEBUG_FUNCTION_NAME);


    if ( batb.run.keyset.u->click() ) run.toggle_a = !run.toggle_a;
    if ( batb.run.keyset.i->click() ) {run.toggle_b = !run.toggle_b; std::cout << "FPS: " << env::frame_fps() << std::endl; }
    if ( batb.run.keyset.ogre->click() ) run.toggle_ogre = !run.toggle_ogre;
    if ( batb.run.keyset.nanovg->click() ) run.toggle_nanovg = !run.toggle_nanovg;
    if ( batb.run.keyset.tb->click() ) run.toggle_tb = !run.toggle_tb;

    // forest
    forest_stack.iterate( *forest );

    // Ogre demo:
    if ( run.toggle_ogre )
    {
        tmp::ogre::demo_iterate( batb, run, *forest );
    }

    // nanovg demo:
    if ( run.toggle_nanovg )
    {
        tmp::nanovg::demo_iterate( false, false );
    }
    
    // ALURE demo:
    tmp::al::demo_iterate( batb, run );

    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    // 


    if ( batb.run.keyset.pause->click() )
    {
        // escaping from IterationMain finishes iteration.
        batb.log << "IterationRunMain ->" << std::endl;
        return stack.finish();
    }

    // run old-BATB if old-key released
    if ( batb.run.keyset.old->released() )
    {
        // remove window
        //if ( widget_->GetParent() != nullptr )
        if ( batb.run.guiMain->GetParent() != nullptr )
        {
            //batb.gui.root.RemoveChild( widget_ );
            batb.gui.root.RemoveChild( batb.run.guiMain );
        }

        batb.log << "IterationRunMain -> IterationRunOld" << std::endl;
        return stack.next(  game::begin_iteration( batb.run.iterationRunOld ), 
                            game::begin_iteration( *this ) );
    }



    // continue with this itertion, unless stack handled
    return stack.next( this );

}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationRunMain& iter)
{
    BATB_LOG_FUNC( iter.batb );

    // create GUI widget. GUI is up and running...
    // NOTE: memory management is performed by TB! NO!!:
    // FIXME: memory leak, according to valgring
    auto* window = new tb::TBWindow();
    window->SetSettings( tb::WINDOW_SETTINGS_TITLEBAR | tb::WINDOW_SETTINGS_RESIZABLE | tb::WINDOW_SETTINGS_CAN_ACTIVATE );
    window->SetSize(250, 630 );
    //window->Position( 40, 40 );

    window->SetText( "testing turbobadger. a window." );
    window->SetGravity( tb::WIDGET_GRAVITY_ALL );
    iter.widget_ = window;

}

void end(IterationRunMain& iter)
{
    BATB_LOG_FUNC( iter.batb );
}



} // namespace run

} // namespace batb


