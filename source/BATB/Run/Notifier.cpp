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
#include "BATB/Run/Notifier.hpp"
#include "BATB/Run.hpp"
#include "BATB/Run/World.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/GUI.hpp"
#include "BATB/Time.hpp"



  
namespace batb
{

namespace run
{

////////////////////////////////////////////////////////////////////////////////
//

void Notifier::begin()
{

    tb_notify_ = new TBNotify( batb );

    // add to screen
    batb->gui->addWidgetTop( tb_notify_ );


}

void Notifier::end()
{
    batb->gui->removeWidget( tb_notify_ );
    
    delete tb_notify_;
    tb_notify_ = nullptr;
}

void Notifier::step(World& run)
{
 
    // remove completed messages
    auto i = std::begin( messages_ );
    while ( i != std::end( messages_ ) )
    {
        if ( i->finished_ )
        {
            // send event 
            //event::NotifyMessageComplete event( &( *i ) );
            //run.pushEvent( event );

            i = messages_.erase( i );
        }
        else
        {
            ++i;
        }
    }


    // update GUI
    tb_notify_->step( run );
}

bool Notifier::message(const NotifyMessage& m)
{
    NotifyMessage msg = m;
    msg.tick = batb->time->get();

    messages_.push_back( msg );
    NotifyMessage* ptr = &messages_.back();

    // add UI-object
    tb_notify_->push( ptr );

    return true;
}



} // namespace run

} // namespace batb

