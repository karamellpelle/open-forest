//    open-demo: an orientering game.
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
#include "BATB/Demo.hpp"
#include "BATB/Demo/World.hpp"
#include "BATB/Demo/BeginEvents.hpp"


namespace batb
{

namespace demo
{

void BeginEvents::operator()(World& demo)
{
    // free old events
    events_step( demo.events );

    
    // move events from demo::Run over to demo::World
    // all events (from Run and subclasses of IterationRun (henc
    // there is no need for EventList's in subclasses of IterationRun))
    // are propagated down to demo::World.
    // 
    // ideally, events should not be taken but instead copied.
    // however, events can not be doubled (yet, because of current
    // implementation of events_step). 
    // hence only 1 demo::World will receive the events from Run
    // each frame. in practice this is no problem, since we will
    // only work on 1 demo::World each frame (and always!)
    //
    //world.events.push( batb.demo.events_ ); 
    //demo.events.take( batb.demo.events_ );
}

} // namespace demo

} // namespace batb



