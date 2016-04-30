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
#include "batb/value/run.hpp"
#include "tb/animation/tb_widget_animation.h"



  
namespace batb
{

namespace run
{

void Notify::step(World& run)
{
 
    // remove completed messages
    for ( auto i = std::begin( messages_ ); i != std::end( messages_ ); ++i )
    {

        if ( i->finished_ )
        {
            // send event 
            event::NotifyMessageComplete event( &( *i ) );
            run.events.push( event );

            messages_.erase( i );
            
        }
    }


    auto wth = run.scene.wth;
    auto hth = run.scene.hth;

    // fasten widget to root 
    tb::TBRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = wth;
    rect.h = hth;
    tb_notify->SetRect( rect );

    // update GUI
    tb_notify->step( run );
}

bool Notify::operator()(const NotifyMessage& m)
{
    NotifyMessage msg = m;
    msg.tick = batb.env.tick();

    messages_.push_back( msg );
    NotifyMessage* ptr = &messages_.back();

    // add UI-object
    tb_notify->push( ptr );

    return true;
}


////////////////////////////////////////////////////////////////////////////////
//

void begin(Notify& notify)
{
    BATB& batb = notify.batb;

    notify.tb_notify = new TBNotify( notify );

    // add to screen
    batb.gui.addWidget( notify.tb_notify );


}

void end(Notify& notify)
{
    BATB& batb = notify.batb;
    
    batb.gui.removeWidget( notify.tb_notify );
    
    delete notify.tb_notify;
    notify.tb_notify = nullptr;
}

} // namespace run

} // namespace batb

