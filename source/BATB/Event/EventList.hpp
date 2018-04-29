//    open-forest: an orienteering game.
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
#ifndef BATB_EVENT_EVENTLIST_HPP
#define BATB_EVENT_EVENTLIST_HPP
#include "BATB/Event/EventBase.hpp"





namespace batb
{

class EventList : public std::list<std::shared_ptr<EventBase>> // private inheritance?
{
friend class EventEaterSet;

public:
    // push event, copying given data 'd' (very suitable for small classes for example enum's)
    template <typename T>
    void push(const T& d, uint frames = 1) // enable_if is_reference
    {
        if ( frames )
        {
            auto ptr = std::make_shared<EventDataCopy<T>>( d );
            ptr->frames( frames );
            push_back( ptr );
        }
    }

    // push event, with pointer to data 'ptr_d' and releasing when finished 
    // (delete pointer with if releaser not given) 
    template <typename T, typename D = std::default_delete<T>> // enable_if is_pointer
    void push(T* ptr_d, const D& del = D(), uint frames = 1)
    {
        if ( frames )
        {
            auto ptr = std::make_shared<EventDataPoint<T>>( ptr_d, del );
            ptr->frames( frames );
            push_back( ptr );
        }
    }
    
    // push events from event list
    //template <>
    //void push(const EventList& es)
    //{
    //    std::copy( es.begin(), es.end(), std::back_inserter( );
    //}
    // NOTE: it is not allowed (yet) to copy events from one EventList to another.
    //       because the function 'EventList::step' below will decrease frame life more
    //       than once each frame if we step events more than once each. therefore
    //       we create this function 'take'. this is something similar to 'std::move'
    //       although we don't invalidate the other object, we only move the events
    //       from the other object into this
    //

    // push and clear
    void take(EventList& es)
    {
        std::copy( es.begin(), es.end(), std::back_inserter( *this ) );
        es.clear();
    }

    void step();

    // TODO?
    void tick(tick_t t) { tick_ = t; }
    void frame(uint n)  { frame_ = n; }

private:
    tick_t tick_ = 0.0;
    uint frame_ = 0;


};


////////////////////////////////////////////////////////////////////////////////

// step and remove dead events
inline void EventList::step()
{
    auto pred = [](const std::shared_ptr<EventBase>& e)
    {
        // we are not allowed to modify e (see cppreference.com of list::remove_if)
        // but we can surely modify the object it points to!
        e->frame_lifes = e->frame_lifes == 0 ? 0 : e->frame_lifes - 1;

        // remove iff current frame is the last frame for event
        return e->frame_lifes == 0;

    };

    // step and/or remove (including free memory, cf. shared_ptr) events
    remove_if( pred );

}


} // namespace batb


#endif

