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
#include "batb/run.hpp"
#include "batb/run/World.hpp"
#include "batb/run/notify/Notify.hpp"
#include "batb/run/notify/TBNotify.hpp"
#include "batb/value/run.hpp"
#include "tb/animation/tb_widget_animation.h"



  
namespace batb
{

namespace run
{

TBNotify::TBNotify(Notify& notify) : batb( notify.batb ), notify_( notify )
{
    /*
    using namespace tb;

    // read file as node tree, letting us parse custom nodes for this widget.
    // see tb_widgets_reader.[hc]pp
    TBNode node;
    if ( node.ReadFile( "static://batb/run/console.tb.txt" ) )
    {
        // let TB populate this TBWindow from file
        g_widgets_reader->LoadNodeTree( this, &node );

        // read properties for 'this' (g_widgets_reader only adds children of 'this')
        // (see void TBWidget::OnInflate(const INFLATE_INFO &info) in tb_widgets_reader.cpp)
        SetSkinBg( node.GetValueString( "skin", "TBWindow" ) );
        //SetOpacity( node.GetValueFloat("opacity", GetOpacity()) );

        // we can now retrieve child widgets of 'this' from ID with
        // - MyWidget* widget = GetWidgetByIDAndType<MyWidget>( TBIDC("my-id-name") );
        // - tb::TBWidget* widget = GetWidgetById( TBIDC( "my-id-name" ) );
        if ( (tb_input_ = GetWidgetByIDAndType<TBEditFieldEnter>( TBIDC( "input" ) ) ) == nullptr )
        {
            batb.log << "TBNotify: "
                     << "no TBEditFieldEnter 'input' defined :("
                     << std::endl;
        }
        if ( (tb_output_ = GetWidgetByIDAndType<TBEditField>( TBIDC( "output" ) ) ) == nullptr )
        {
            batb.log << "TBNotify: "
                     << "no TBEditField 'output' defined :("
                     << std::endl;
        }
        // , and read custom configuration from TBNode; see Demo01.cpp

        // only input should receive focus
        tb_output_->SetIsFocusable( false );

        //tb_input_->SetPlaceholderText( "enter command" );
    }
    else
    {
        batb.log << "TBNotify: "
                 << "could not read tbconsole.tb.txt" 
                 << std::endl;

        //throw std::runtime_error( "TBNotify: error reading file" );
    }

    // settings:
    //SetSettings( tb::WINDOW_SETTINGS_RESIZABLE );
    SetSettings( tb::WINDOW_SETTINGS_NONE );
    SetText( "TBNotify" );
    */
}


void TBNotify::step(World& run)
{
    for ( auto i : tb_notify_messages_ )
    {
        auto dur = i->message->duration;
        if ( dur != 0 )
        {
            tick_t tick = i->message->tick + i->message->duration;
            if ( batb.env.tick() <= tick ) 
            {
                // TODO: remove 'i'
                // message->finish() and remove elemnet from list
            }
        }
    }
}

void TBNotify::push(NotifyMessage* m)
{
    tb_notify_messages_.push_back( std::make_shared<TBNotifyMessage>( this, m ) );

}


TBNotifyMessage::TBNotifyMessage(TBNotify* n, NotifyMessage* m) : tb_notify( n ), message( m )
{

}


//void TBNotify::OnFocusChanged(bool focus)
//{
//    if ( focus )
//    {
//        // hopefully, this does not mess up (see TBWidgets::SetFocus())
//        SetFocusRecursive( tb::WIDGET_FOCUS_REASON_UNKNOWN );
//    }
//}
//
//bool TBNotify::OnEvent(const tb::TBWidgetEvent& event)
//{
//
//}
//


} // namespace run

} // namespace batb

