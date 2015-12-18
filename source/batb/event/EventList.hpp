//    open-forest: an orienteering game.
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
#ifndef BATB_EVENT_EVENT_LIST_HPP
#define BATB_EVENT_EVENT_LIST_HPP
#include "batb/event/EventBase.hpp"
#include "batb/event/Event.hpp"





namespace batb
{

class EventList
{
friend class EventEaterSet;
friend void step(EventList& );

public:
    // push new event (copy data)
    template <typename T>
    void push(const T& d) // enable_if is_reference
    {
        events_.push_back( std::make_shared<EventDataCopy<T>>( d ) );
    }

    // push new event (point to data)
    template <typename T, typename D = std::default_delete<T>> // enable_if is_pointer
    void push(T* d, const D& del = D())
    {
        events_.push_back( std::make_shared<EventDataPoint<T>>( d, del ) );
    }
    
    // push events from event list
    void push(const EventList& es)
    {
        std::copy( es.events_.begin(), es.events_.end(), std::back_inserter( events_ ) );
    }
    // push and clear
    void move(EventList& es)
    {
        push( es );
        es.events_.clear();
    }



    void set_tick(tick_t t) { tick_ = t; }
    void set_frame(uint n) { frame_ = n; }

private:
    tick_t tick_ = 0.0;
    uint frame_ = 0;

    //std::list<std::unique_ptr<EventBase>> events_;
    std::list<std::shared_ptr<EventBase>> events_; // TODO: list of Event's

    static bool step_lifes(const std::shared_ptr<EventBase>& e)
    {
        if ( e->frame_lifes == 0 )
        {
            return true;
        }
        // we are not allowed to modify e (cppreference.com of list::remove_if)
        // but we can surely modify the object pointed too!
        --(e->frame_lifes);
        return false;
    }
};


////////////////////////////////////////////////////////////////////////////////

inline void step(EventList& list)
{
    // free mem iff frame_lifes == 0
    //events_.remove_if( [](auto e) { return e->frame_lifes == 0; } ); // c++14
    list.events_.remove_if( EventList::step_lifes );

}


} // namespace batb


#endif

