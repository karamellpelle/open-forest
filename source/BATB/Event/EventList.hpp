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
friend void events_step(EventList& );

public:
    // push new event (copy data)
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

    // push new event (point to data)
    template <typename T, typename D = std::default_delete<T>> // enable_if is_pointer
    void push(T* d, const D& del = D(), uint frames = 1)
    {
        if ( frames )
        {
            auto ptr = std::make_shared<EventDataPoint<T>>( d, del );
            ptr->frames( frames );
            push_back( ptr );
        }
    }
    
    // NOTE: it is not allowed (yet) to copy events from one EventList to another.
    //       because the function 'events_step' below will decrease frame life more
    //       than once each frame, if events_step more than once each frame.
    //       hence only use 'take'
    //
    // push events from event list
    //void push(const EventList& es)
    //{
    //    std::copy( es.begin(), es.end(), std::back_inserter( );
    //}

    // push and clear
    void take(EventList& es)
    {
        std::copy( es.begin(), es.end(), std::back_inserter( *this ) );
        es.clear();
    }


    // TODO?
    void set_tick(tick_t t) { tick_ = t; }
    void set_frame(uint n) { frame_ = n; }

private:
    tick_t tick_ = 0.0;
    uint frame_ = 0;


    static bool step_lifes(const std::shared_ptr<EventBase>& event)
    {
        // we are not allowed to modify e (cppreference.com of list::remove_if)
        // but we can surely modify the object it points to!
        auto& lifes = event->frame_lifes;

        lifes = lifes == 0 ? 0 : lifes - 1; // but lifes should never be empty!

        // remove iff current frame is the last frame for event
        return lifes == 0;
    }
};


////////////////////////////////////////////////////////////////////////////////

inline void events_step(EventList& list)
{
    // free mem iff frame_lifes == 0
    //events_.remove_if( [](auto e) { return e->frame_lifes == 0; } ); // c++14
    list.remove_if( EventList::step_lifes );

}


} // namespace batb


#endif

