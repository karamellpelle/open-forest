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
#ifndef BATB_EVENT_EVENT_HPP
#define BATB_EVENT_EVENT_HPP
#include "batb/event/EventBase.hpp"


namespace batb
{


////////////////////////////////////////////////////////////////////////////////
// Event
//
// user does not handle EventBase directly
//
class Event
{
template <typename T>
friend T* eat(const Event& e);
friend class EventEater;

public:
    //template <typename T>
    //Event(const T& ) : Event( std::make_shared<EventDataCopy<T>>( d ) ) { }
    //template <typename T, typename D = std::default_delete<T>> // enable_if is_pointer
    //Event(T* d, const D& del = D()) : Event( std::make_shared<EventDataPoint<T>>( d, del ) ) { }

    Event(const Event& ) = default;
    Event& operator=(const Event& ) = default;
    Event(Event&& ) = default;

private:
    Event(const std::shared_ptr<EventBase>& b) : base_( b ) { } 

    std::shared_ptr<EventBase> base_;
};



////////////////////////////////////////////////////////////////////////////////
// try to eat single event as type T
//
// this is the thing that makes Event useful
// no ownership to returned object shall be acclaimed
//
template <typename T>
T* eat(const Event& e)
{
    return std::type_index( typeid( T ) ) == e.base_->type() ? 
           static_cast<T*>( e.base_->data() ) : nullptr;
}





} // namespace batb


#endif

