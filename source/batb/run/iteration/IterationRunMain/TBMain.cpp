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
#include "batb/run/iteration/IterationRunMain/TBMain.hpp"
#include "batb/run/events.hpp"

namespace batb
{

namespace run
{


TBMain::TBMain(BATB& b) : batb( b )
{

    // read file as node tree, letting us parse custom nodes for this widget.
    // (see Demo01.cpp)
    tb::TBNode node;
    if ( node.ReadFile( "static://batb/run/main.tb.txt" ) )
    {
        // let TB populate this TBWindow from file
	tb::g_widgets_reader->LoadNodeTree( this, &node );

        // we can now retrieve child widgets of 'this' from ID with
        // - MyWidget* widget = GetWidgetByIDAndType<MyWidget>( TBIDC("my-id-name") );

    }
    else
    {
        batb.log << "TBMain: could not read main.tb.txt" << std::endl;
    }

    // set TB window settings:
    SetSettings( /*tb::WINDOW_SETTINGS_TITLEBAR |*/ tb::WINDOW_SETTINGS_RESIZABLE /*| tb::WINDOW_SETTINGS_CAN_ACTIVATE*/ );
    SetText( "TBMain" );
    //Invalidate();
}


bool TBMain::OnEvent(const tb::TBWidgetEvent& ev)
{
    // TODO: push events to batb.run

    if ( ev.type == tb::EVENT_TYPE_CLICK )
    {
        tb::TBID id = ev.target->GetID(); 

        if ( id == tb::TBIDC( "do-demo-forest" ) )
        {
            batb.run.pushEvent( event::Do::DemoForest );
            return true;
        }
        if ( id == tb::TBIDC( "do-nanovg" ) )
        {
            batb.run.pushEvent( event::Do::NanoVG );
            return true;
        }
        if ( id == tb::TBIDC( "do-old" ) )
        {
            batb.run.pushEvent( event::Do::Old );
            return true;
        }
        if ( id == tb::TBIDC( "do-exit" ) )
        {
            batb.run.pushEvent( event::Do::Exit );
            return true;
        }
    } 

    return tb::TBWindow::OnEvent( ev );
}



} // namespace run

} // namespace batb




