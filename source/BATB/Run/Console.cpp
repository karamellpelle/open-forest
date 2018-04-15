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
#include "BATB/Player.hpp"
#include "BATB/Run.hpp"
#include "BATB/Run/World.hpp"
#include "BATB/Run/Console.hpp"
#include "BATB/Run/Console/TBConsole.hpp"
#include "BATB/Run/Console/parse.hpp"
#include "BATB/Run/Console/cmd.hpp"
#include "BATB/Value/Run.hpp"
#include "tb/animation/tb_widget_animation.h"
#include "tb/tb_editfield.h"




  
namespace batb
{

namespace run
{

////////////////////////////////////////////////////////////////////////////////
//

void Console::begin()
{
    tb_console = new TBConsole( batb );

    // add to screen
    batb->gui->addWidget( tb_console );


}

void Console::end()
{
    
    batb->gui->removeWidget( tb_console );

    delete tb_console;
    tb_console = nullptr;
}

void Console::step(World& run)
{
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;
    auto rect = tb_console->GetRect();

    // fasten console widget (height preserved)
    rect.x = 0;
    rect.y = 0;
    rect.w = wth;
    tb_console->SetRect( rect );

    // always on top
    tb_console->SetZ( tb::WIDGET_Z_TOP ); // at top of all others

    tb_console->step( run );

    // for now, set PS1 here
    std::ostringstream os;
    os << run.player->name << "> ",
    ps1_ = os.str(); 
}

void Console::open(World& run)
{

    tb_console->SetVisibility( tb::WIDGET_VISIBILITY_VISIBLE );


    // remove current animation
    // this deletes the running animation (see source)
    tb::TBWidgetsAnimationManager::AbortAnimations( tb_console );

    // Start move animation
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;
    tb::TBRect rect0 = tb_console->GetRect();
    tb::TBRect rect1( 0, 0, wth, hth ); // 'step' above will correct wth (however, not hth)
    if ( auto anim = new tb::TBWidgetAnimationRect( tb_console, rect0, rect1 ) )
            tb::TBAnimationManager::StartAnimation( anim, tb::ANIMATION_CURVE_SLOW_DOWN, 1000.0 * value::runConsoleOpenTicks );

    // set focus 
    tb_console->SetIsFocusable( true );
    tb_console->SetFocus( tb::WIDGET_FOCUS_REASON_UNKNOWN );

    // ensure we can write without interfere with KeySet's
    batb->gui->lockKeys( true );
    
}

void Console::close(World& run)
{

    // remove current animation
    // this deletes the running animation (see source)
    tb::TBWidgetsAnimationManager::AbortAnimations( tb_console );

    // Start move animation
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;
    tb::TBRect rect0 = tb_console->GetRect();
    tb::TBRect rect1( 0, 0, wth, 0 ); // 'step' above corrects wth
    if ( auto anim = new tb::TBWidgetAnimationRect( tb_console, rect0, rect1 ) )
            tb::TBAnimationManager::StartAnimation( anim, tb::ANIMATION_CURVE_SLOW_DOWN, 1000.0 * value::runConsoleOpenTicks );

    // unfocus input field
    tb::TBWidget::focused_widget = nullptr;
    tb_console->SetIsFocusable( false );
    
    // enable KeySet's
    batb->gui->lockKeys( false );
}

bool Console::cmd(const std::string& input)
{
    // output PS1 + typed command line
    *this << getPS1() << input << "\n";

    // easteregg command typed?
    bool (cmd_easteregg)(BATB* , const std::string& );
    if ( cmd_easteregg( batb, input ) ) return true;
    
    std::string in = input;
    std::string cmd = word( in );
    

    // handle command
    if ( cmd == "" )          return true;                    // empty command is OK
    if ( cmd == "echo" )      return cmd_echo( batb, in );   
    if ( cmd == "value" )     return cmd_value( batb, in );
    if ( cmd == ":quit" )     return cmd_quit( batb, in );


    *this << "Console: command not found: " << cmd << std::endl;
    return false;
  
}

std::string Console::getPS1()
{
    return ps1_;
}



////////////////////////////////////////////////////////////////////////////////
// ConsoleStreambuf
//

std::streamsize ConsoleStreambuf::xsputn(const char* s, std::streamsize n)
{
    auto style = console_.tb_console->tb_output_->GetStyleEdit();

    style->AppendText( s, n );
    style->ScrollIfNeeded();

    return n;
}

int ConsoleStreambuf::overflow(int c)
{
    auto style = console_.tb_console->tb_output_->GetStyleEdit();

    char c_ = (char)( c ); 
    // FIXME: handle EOF?
    // FIXME: find out it this is correct:
    style->AppendText( &c_, 1 );
   
    return (int)( c_ );
}

} // namespace run

} // namespace batb

