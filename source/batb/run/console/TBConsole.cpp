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
#include "batb/run/console/TBConsole.hpp"
#include "batb/run/console/Console.hpp"
#include "batb/run/Run.hpp"
#include "tb/tb_widgets_common.h"
#include "tb/tb_editfield.h"
#include "batb.hpp"

namespace batb
{

namespace run
{

TBConsole::TBConsole(BATB* b) : batb( b )
{
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
        if ( (tb_input_ = GetWidgetByIDAndType<TBEditField>( TBIDC( "input" ) ) ) == nullptr )
        {
            batb->log << "TBConsole: "
                     << "no TBEditField 'input' defined :("
                     << std::endl;
        }
        if ( (tb_output_ = GetWidgetByIDAndType<TBEditField>( TBIDC( "output" ) ) ) == nullptr )
        {
            batb->log << "TBConsole: "
                     << "no TBEditField 'output' defined :("
                     << std::endl;
        }
        {
            // only input should receive focus
            tb_output_->SetIsFocusable( false );
            // TODO: change skin of scrollbar. use TBEditField::GetScrollRoot() ?
            
        }


        //tb_input_->SetPlaceholderText( "enter command" );
    }
    else
    {
        batb->log << "TBConsole: "
                 << "could not read tbconsole.tb.txt" 
                 << std::endl;

        //throw std::runtime_error( "TBConsole: error reading file" );
    }

    // settings:
    //SetSettings( tb::WINDOW_SETTINGS_RESIZABLE );
    SetSettings( tb::WINDOW_SETTINGS_NONE );
    SetText( "TBConsole" );
}


void TBConsole::OnFocusChanged(bool focus)
{
    if ( focus )
    {
        // hopefully, this does not mess up (see TBWidgets::SetFocus())
        SetFocusRecursive( tb::WIDGET_FOCUS_REASON_UNKNOWN );
    }
}

bool TBConsole::OnEvent(const tb::TBWidgetEvent& event)
{

    if ( event.type == tb::EVENT_TYPE_KEY_DOWN )
    {
        // enter key down?
        if ( event.special_key == tb::TB_KEY_ENTER )
        {
            // note: newline is not added!
            tb::TBStr tbstr;
            tb_input_->GetStyleEdit()->GetText( tbstr );

            std::string str = tbstr.CStr();

            // save to history
            // TODO: only push if not equal to previous
            history_.push_back( str );
            history_ix_ = history_.size();

            history_current_.clear();

            tb_input_->GetStyleEdit()->Clear();

            // send command to Console
            batb->run->console->cmd( str );


            return true;

        }
        if ( event.special_key == tb::TB_KEY_UP )
        {
            history_up();
            return true;
        }
        if ( event.special_key == tb::TB_KEY_DOWN )
        {
            history_down();
            return true;
        }

    }
    //if ( event.type == tb::EVENT_TYPE_CHANGED )
    //{
    //    return true;
    //}


    return tb::TBWindow::OnEvent( event );
}

void TBConsole::step(World& run)
{
}

void TBConsole::operator()(const std::string& str)
{
    tb_output_->GetStyleEdit()->AppendText( str.c_str(), str.size() );
    tb_output_->GetStyleEdit()->ScrollIfNeeded();
}

void TBConsole::clear()
{
    tb_output_->GetStyleEdit()->Clear();
    
    // clear history too
    history_current_.clear();
    history_.clear();
    history_ix_ = 0;
}

void TBConsole::history_up()
{
    // TODO: search backwards for first matching history_current_

    tb::TBStr str;
    tb_input_->GetStyleEdit()->GetText( str );

    // make sure to save current line
    if ( history_ix_ == history_.size() )
    {
        history_current_ = str.CStr();
    }

    if ( history_ix_ != 0 )
    {
        --history_ix_;
        auto* str = history_[ history_ix_ ].c_str();

        tb_input_->GetStyleEdit()->SetText( str , tb::TB_CARET_POS_END );
    }
}

void TBConsole::history_down()
{
    // TODO: search forwards for first matching history_current_

    if ( history_ix_ != history_.size() )
    {
        ++history_ix_;
        const char* str = history_ix_ == history_.size() ?
                          history_current_.c_str() : history_[ history_ix_ ].c_str();

        tb_input_->GetStyleEdit()->SetText( str, tb::TB_CARET_POS_END  );
    }
}

} // namespace run

} // namespace batb




