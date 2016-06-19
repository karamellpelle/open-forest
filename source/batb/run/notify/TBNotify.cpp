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
#include "tb/tb_editfield.h"




  
namespace batb
{

namespace run
{

TBNotify::TBNotify(BATB& b) : tb::TBLayout( tb::AXIS_Y ), batb( b ), notify_( b.run.notify )
{
    // set layout parameters
    
    // size: preferred
    //SetLayoutSize( tb::LAYOUT_SIZE_PREFERRED );
    // position: center
    SetLayoutPosition( tb::LAYOUT_POSITION_CENTER );
    // distribution: preferred
    SetLayoutDistribution( tb::LAYOUT_DISTRIBUTION_PREFERRED );
    // distribution position: top
    SetLayoutDistributionPosition( tb::LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP );

    // clip
    SetLayoutOverflow( tb::LAYOUT_OVERFLOW_CLIP );

    // spacing
	/** Set the spacing between widgets in this layout. Setting the default (SPACING_FROM_SKIN)
		will make it use the spacing specified in the skin. */
	//void SetSpacing(int spacing);

    // since this is a non-interactive top widget spanning the whole screen,
    // make sure it does not steal input
    SetIsFocusable( false );
    SetIgnoreInput( true );

    SetSize( 220, 800 );
    // this makes widget fill to root automatically:
    //SetGravity( tb:WIDGET_GRAVITY_ALL );
}

TBNotifyMessage::TBNotifyMessage(TBNotify* n, NotifyMessage* msg) : tb::TBWidget(), tb_notify( n ), message( msg )
{
    using namespace tb;

    BATB& batb = tb_notify->batb;

    // read file as node tree, letting us parse custom nodes for this widget.
    // see tb_widgets_reader.[hc]pp
    TBNode node;
    if ( node.ReadFile( "static://batb/run/notifymessage.tb.txt" ) )
    {
	if (const char *skin = node.GetValueString("skin", nullptr))
	{
	    SetSkinBg(skin);
	}

        // let TB populate this TBWindow from file
        g_widgets_reader->LoadNodeTree( this, &node );

        //SetSkinBg( node.GetValueString( "skin", "TBWindow" ) ); // TODO!

        if ( ( edit = GetWidgetByIDAndType<TBEditField>( TBIDC( "edit" ) ) ) )
        {
            // set text
            edit->SetText( msg->str.c_str() );

        }
        else
        {
            batb.log << "TBNotifyMessage: 'edit' not defined\n";
        }

        // only input should receive focus
        //tb_output_->SetIsFocusable( false );
        
        // TODO: set preferred size!

    }
    else
    {
        batb.log << "TBNotify: could not read notifymessage.tb.txt\n" ;

    }

    // hardcoded size for now!
    SetSize( 200, 100 );
}


void TBNotify::step(World& run)
{
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;

    // span out this layout to the whole screen
    // FIXME: fill to root automatically, SetGravity all
    SetRect( tb::TBRect(0, 0, wth, hth) );

    tick_t tick = batb.env.tick();
    

    auto i = std::begin( tb_notify_messages_ );
    while ( i != std::end( tb_notify_messages_ ) )
    {
        TBNotifyMessage* tbmsg = *i;
        bool remove = false;

        auto message = tbmsg->message;
        tick_t ts = message->tick + message->duration;

        // duration specifies minimum time
        if ( ts <= tick ) 
        {
            if ( message->key )
            {
                if ( message->key->is_down() )
                {
                    remove = true;
                }
            }
            else
            {
                remove = true;
            }
        }
        if ( remove )
        {
            message->finish();

            // TODO: animate
            RemoveChild( tbmsg );
            delete tbmsg;

            i = tb_notify_messages_.erase( i );    
        }
        else
        {
            ++i;
        }
    }

}

void TBNotify::push(NotifyMessage* m)
{
    auto ptr = new TBNotifyMessage( this, m );
    
    tb_notify_messages_.push_back( ptr );

    // add as child widget
    AddChild( ptr );
    

    
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

