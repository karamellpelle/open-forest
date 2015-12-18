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
#include "batb/run/iteration/IterationRunMain/RunMainTBWidget.hpp"
#include "batb.hpp"

namespace batb
{

namespace run
{

RunMainTBWidget::RunMainTBWidget(IterationRunMain& i) : iteration( i )
{
    using namespace tb;

    // read file as node tree, letting us parse custom nodes for this widget.
    // (see Demo01.cpp)
    TBNode node;
    //if ( node.ReadFile( file::static_data( "batb/run/gui/main.tb.txt" ).c_str() ) )
    //if ( node.ReadFile( "Demo/demo01/ui_resources/test_connections.tb.txt" ) )
    //static const char* path =  "batb/gui/Demo/demo01/ui_resources/test_connections.tb.txt";
    static const char* path = "batb/run/iteration/IterationRunMain/runmain.tb.txt"; 
    if ( node.ReadFile( file::static_data( path ).c_str() ) )
    {
        // let TB populate this TBWindow from file
	g_widgets_reader->LoadNodeTree( this, &node );

        // we can now retrieve child widgets of 'this' from ID with
        // - MyWidget* widget = GetWidgetByIDAndType<MyWidget>( TBIDC("my-id-name") );
        // - tb::TBWidget* widget = GetWidgetById( TBIDC( "my-id-name" ) );
        TBWidget* w0 = GetWidgetByID( TBIDC("test-list") );
        TBWidget* w1 = GetWidgetByID( TBIDC("test-image") );

        // , and read custom configuration from file:
        //
        // ...

    }
    else
    {
        iteration.batb.log << "RunMainTBWidget: could not read associated file." << std::endl;

        throw std::runtime_error( "RunMainTBWidget: error reading file" );
    }

    // set TB window settings:
    SetSettings( tb::WINDOW_SETTINGS_TITLEBAR | tb::WINDOW_SETTINGS_RESIZABLE | tb::WINDOW_SETTINGS_CAN_ACTIVATE );
    SetSize( 360, 360  );
    SetPosition( TBPoint( 30, 30 ) );
    SetText( "RunMainTBWidget" );
    SetOpacity(0.50f);
    //SetGravity( tb::WIDGET_GRAVITY_ALL );
    //ResizeToFitContent();
    EnsureFocus();
}


bool RunMainTBWidget::OnEvent(const tb::TBWidgetEvent& ev)
{
    using namespace tb; 
    // TODO: push events to batb.run
    if ( ev.type == EVENT_TYPE_CLICK )
    {
        TBID id = ev.target->GetID(); 
        if ( id == TBIDC( "run-old" ) )
        {
            iteration.batb.log << "RunMainTBWidget -> \"run old-BATB\"" << std::endl; 
            return true;
        }
        if ( id == TBIDC( "run-nanovg" ) )
        {
            iteration.batb.log << "RunMainTBWidget -> \"run nanovg\"" << std::endl; 
            return true;
        }
        if ( id == TBIDC( "exit" ) )
        {
            iteration.batb.log << "RunMainTBWidget -> \"exit\"" << std::endl;
            return true;
        }
    } 
    // EVENT_TYPE_CHANGED...
    //

    return tb::TBWindow::OnEvent( ev );
}

void RunMainTBWidget::OnMessageReceived(tb::TBMessage* msg)
{
    using namespace tb;

    //TBID id = msg->message;
    //TBMessageData* data = msg->data;

    // handle data from id...
}



} // namespace run

} // namespace batb




