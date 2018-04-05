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
#include "batb/forest/BeginEvents.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb.hpp"


namespace batb
{

namespace forest
{

void BeginEvents::operator()(World& forest)
{
    events_step( forest.events );

    // move events from forest::Forest over to forest::World
    // all events (from Forest and subclasses of IterationForest (henc
    // there is no need for EventList's in subclasses of IterationForest))
    // are propagated down to forest::World.
    // 
    // ideally, events should not be taken but instead copied.
    // however, events can not be doubled (yet, because of current
    // implementation of events_step). 
    // hence only 1 forest::World will receive the events from Forest
    // each frame. in practice this is no problem, since we will
    // only work on 1 forest::World each frame (and always!)
    //
    //world.events.push( batb.forest.events_ ); 
    //forest.events.take( batb.forest.events_ );

}

} // namespace forest

} // namespace batb



