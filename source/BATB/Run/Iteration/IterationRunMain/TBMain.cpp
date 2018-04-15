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
#include "BATB/Run/World.hpp"
#include "BATB/Run/Iteration/IterationRunMain/TBMain.hpp"
#include "BATB/Run/events.hpp"

namespace batb
{

namespace run
{


TBMain::TBMain(BATB* b) : batb( b )
{
    
    constexpr const char* path = "static://BATB/Run/main.tb.txt";

    // read file as node tree, letting us parse custom nodes for this widget.
    // (see Demo01.cpp)
    tb::TBNode node;
    if ( node.ReadFile( path ) )
    {
        // let TB populate this TBWindow from file
	tb::g_widgets_reader->LoadNodeTree( this, &node );

        // we can now retrieve child widgets of 'this' from ID with
        // - MyWidget* widget = GetWidgetByIDAndType<MyWidget>( TBIDC("my-id-name") );

    }
    else
    {
        batb->log << "TBMain: could not read " << path << std::endl;
    }

    // set TB window settings:
    //SetSettings( tb::WINDOW_SETTINGS_RESIZABLE );
    //SetText( "TBMain" );
}

void TBMain::step(World& run)
{
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;

    // span out this layout to the whole screen
    // FIXME: fill to root automatically
    SetRect( tb::TBRect(0, 0, wth, hth) );
}

bool TBMain::OnEvent(const tb::TBWidgetEvent& ev)
{
    // TODO: push events to batb.run

    if ( ev.type == tb::EVENT_TYPE_CLICK )
    {
        tb::TBID id = ev.target->GetID(); 

        if ( id == TBIDC( "do-demo-forest" ) )
        {
            batb->run->pushEvent( event::Do::DemoForest );
            return true;
        }
        if ( id == TBIDC( "do-nanovg" ) )
        {
            batb->run->pushEvent( event::Do::NanoVG );
            return true;
        }
        if ( id == TBIDC( "do-old" ) )
        {
            batb->run->pushEvent( event::Do::Old );
            return true;
        }
        if ( id == TBIDC( "do-exit" ) )
        {
            batb->run->pushEvent( event::Do::Exit );
            return true;
        }
    } 

    return tb::TBWidget::OnEvent( ev );
}



} // namespace run

} // namespace batb




