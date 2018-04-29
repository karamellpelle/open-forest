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
#include "BATB/Run/Notify.hpp"
#include "BATB/Run/Notify/TBNotify.hpp"
#include "BATB/GUI.hpp"
#include "BATB/GUI/tb/helpers.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/Keys/Key.hpp"
#include "BATB/Time.hpp"



using namespace batb;
using namespace batb::run;

namespace tb
{

////////////////////////////////////////////////////////////////////////////////

// without this, TBNotify is never read (g_widgets_reader->LoadNodeTree( this, &node )):
TB_WIDGET_FACTORY(TBNotifyMessage, TBValue::TYPE_STRING, WIDGET_Z_TOP) { std::cout << "inflating TBNotifyMessage!"<<std::endl;}

////////////////////////////////////////////////////////////////////////////////
// TBNotify
//


TBNotify::TBNotify(BATB* b) : tb::TBLayout( tb::AXIS_Y ), batb( b ), notify_( b->run->notify.get() )
{
    // set layout parameters
    // hardcoded, no .tb.txt here
    
    ////////////////////////////////////////////////////////////////
    // this is a tb::AXIS_Y layout
    // see tb_layout.h for help (this is difficult!)

    // which size should children have?
    // -> size: preferred
    SetLayoutSize( tb::LAYOUT_SIZE_PREFERRED );

    // which x position should children have?
    // -> position: center
    SetLayoutPosition( tb::LAYOUT_POSITION_CENTER );

    // which height should children have when distributing the children?
    // -> distribution: preferred
    SetLayoutDistribution( tb::LAYOUT_DISTRIBUTION_PREFERRED );

    // how should children be moved when distributing the children?
    // -> distribution position: top
    SetLayoutDistributionPosition( tb::LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP );

    // since this is a non-interactive top widget spanning the whole screen,
    // make sure it does not steal input
    SetIsFocusable( false );
    SetIgnoreInput( true );

}



void TBNotifyMessage::message(NotifyMessage* msg)
{
    
    // here it would be nice to have a global variable 'batb'!
    auto batb = tb_notify->batb;

    // what this represents
    notifymessage = msg;

    if ( ( tb_editfield = GetWidgetByIDAndType<TBEditField>( TBIDC( "edit" ) ) ) )
    {
        // set text
        tb_editfield->SetText( msg->str.c_str() );

        // SetTextAlign only works on selections, or TBBlock (i.e. line) of 
        // current position (i.e. beginning), see tb_style_edit.h . 
        // hence we have to select all before aligning
        tb_editfield->GetStyleEdit()->selection.SelectAll();
        tb_editfield->GetStyleEdit()->SetAlign( TB_TEXT_ALIGN_CENTER );
        tb_editfield->GetStyleEdit()->selection.SelectNothing();

    }
    else
    {
        batb->log << "TBNotifyMessage: 'edit' not defined\n";
    }

}


void TBNotify::step(World& run)
{
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;

    // span out this layout to the whole screen
    // FIXME: fill to root automatically, SetGravity all
    SetRect( tb::TBRect(0, 0, wth, hth) );

    tick_t tick = batb->time->get();
    

    auto i = std::begin( tb_notify_messages_ );
    while ( i != std::end( tb_notify_messages_ ) )
    {
        TBNotifyMessage* tbmsg = *i;
        bool remove = false;

        auto message = tbmsg->notifymessage;
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


void TBNotifyMessage::OnInflate(const INFLATE_INFO &info)
{
    // shall we read custom properties?
    // not now.
    TBWidget::OnInflate( info );
}

void TBNotify::push(NotifyMessage* m)
{
    constexpr const char* path = "static://BATB/Run/notifymessage.tb.txt";  

    // read file as node tree, letting us parse custom nodes for this widget.
    // see tb_widgets_reader.[hc]pp
    TBNode node;
    if ( node.ReadFile( path ) )
    {
        // find a TBNotifyMessage definition in file
        for (TBNode* child = node.GetFirstChild(); child; child = child->GetNext())
        {
              if ( std::string( "TBNotifyMessage" ).compare( child->GetName() ) == 0 )
              {
                  // TODO: look at id and compare to NotifyMessage type (warning, tip, etc, new chat, song etc).

                  auto ptr = new_widget<TBNotifyMessage>( this, child );

                  ptr->tb_notify = this;
                  ptr->message( m );

                  // witdh and height are defined in the layout parameters in .tb.txt
                  // here is how:
                  //tb::LayoutParams lp;
                  //lp.min_w = lp.max_w = lp.pref_w = 440;
                  //lp.min_h = 80;
                  //SetLayoutParams( lp );


                  tb_notify_messages_.push_back( ptr );
                  
                  // child created, all done
                  return;

              }
        }
        batb->log << "TBNotify: no TBNotifyMessage defined in " << path << std::endl;
    }
    else
    {
        batb->log << "TBNotify: could not read " << path << std::endl;
    }

    

    
    
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


} // namespace tb

