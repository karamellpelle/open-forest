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
#include "batb/run/Notify.hpp"
//#include "batb/run/Notify/TBNotify.hpp"
#include "batb/value/run.hpp"
#include "tb/animation/tb_widget_animation.h"



  
namespace batb
{

namespace run
{

void Notify::step(World& run)
{
    for ( auto& msg : messages_ )
    {
        // TODO: use batb.run.keyset
        if ( msg.tick_ + msg.duration_ <= run.tick )
        {
            // TODO: remove (or set next)
        }
    }
/*
    auto wth = run.scene.wth;
    auto hth = run.scene.hth;
    auto rect = tb_notify->GetRect();

    // fasten notify widget (height preserved)
    rect.x = 0;
    rect.y = 0;
    rect.w = wth;
    tb_notify->SetRect( rect );

    // always on top
    tb_notify->SetZ( tb::WIDGET_Z_TOP ); // at top of all others

    // possible to change during run. TODO: use skin instead (solid characters)
    tb_notify->SetOpacity( value::runNotifyOpacity );


    // for now, set PS1 here
    std::ostringstream os;
    os << run.player.name << "> ",
    ps1_ = os.str(); 
*/
}

bool Notify::operator()(const NotifyMessage& msg)
{
    
    //messages_.push_back( msg );
    // TODO: add widget
    // TODO: set tick

    return true;
}


////////////////////////////////////////////////////////////////////////////////
//

void begin(Notify& notify)
{
    BATB& batb = notify.batb;

    //notify.tb_notify = new TBNotify( batb );

    // add to screen
    //batb.gui.addWidget( notify.tb_notify );


}

void end(Notify& notify)
{
    BATB& batb = notify.batb;
    
    //batb.gui.removeWidget( notify.tb_notify );
    //
    //delete notify.tb_notify;
    //notify.tb_notify = nullptr;
}

} // namespace run

} // namespace batb

