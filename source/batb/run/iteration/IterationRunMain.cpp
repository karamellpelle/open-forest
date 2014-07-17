//    orienteering-game: an orientering game.
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
    batb.log << THIS_FUNCTION << std::endl;

    // we want clean state for our Key's, no garbage:
    batb.run.keyset.reset();

    // let GUI grab the Keys callbacks
    batb.gui.bind( batb.keys );

    std::cout << "\n\n\n\n";
    std::cout << "press INSERT to toggle old-BATB..." << std::endl;
    std::cout << "click ESCAPE to exit from main..." << std::endl;
    std::cout << "\n\n\n\n";

    // nanovg demo
    tmp::nanovg::demo_begin();


}


void IterationRunMain::iterate_run(IterationStack& stack, World& run)
{
    ////////////////////////////////////////////////////////////////////////////////
    //  OUTPUT
    //


    // nanovg demo:
    static bool premult; static bool blowup;
    if ( batb.run.keyset.u->click() ) premult = !premult;
    if ( batb.run.keyset.i->click() ) blowup = !blowup;
    tmp::nanovg::demo_iterate( premult, blowup );



    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    // 


    if ( batb.run.keyset.pause->click() )
    {
        // escaping from IterationMain finishes iteration.
        batb.log << "IterationRunMain ->" << std::endl;
        return;
    }

    // run old-BATB if old-key released
    if ( batb.run.keyset.old->released() )
    {
        batb.log << "IterationRunMain -> IterationRunOld" << std::endl;
        //stack.next( game::begin_iteration( batb.run.iterationRunOld ), 
        //            game::begin_iteration( this ) );
        return;
    }


    // continue with this itertion, unless stack handled
    stack.next( this );

}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationRunMain& iter)
{
    iter.batb.log << THIS_FUNCTION << std::endl;

    // add windows (GUI is up and running)
    // FIXME: memory management is performed by TB??
    auto* window = new tb::TBWindow();
    window->SetRect( tb::TBRect(0, 0, 128, 64 ) );

    iter.batb.gui.root.AddChild( window );
}

void end(IterationRunMain& iter)
{
    iter.batb.log << THIS_FUNCTION << std::endl;
}



} // namespace run

} // namespace batb


