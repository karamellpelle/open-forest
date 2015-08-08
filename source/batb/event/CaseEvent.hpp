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
#ifndef BATB_EVENT_CASEEVENT_HPP
#define BATB_EVENT_CASEEVENT_HPP
#include "batb/event/EventList.hpp"
#include "batb/event/EventEater.hpp"





namespace batb
{


// Haskell <3 <3 <3
class CaseEvent
{

public:
    CaseEvent(const EventList& es) : es_( es ) { } 
    CaseEvent(const Event& e)
    {
        es_.push( e );
    }

    template <typename... Fns>
    void operator()(Fns... fns)
    {
        // handle each event
        for (const auto& ptr : es_.events_ )
        {
            // eat 'e'
            eat( *ptr, fns... );
        }
    }


private:
    EventList es_;

    template <typename Fn, typename... Fns>
    void eat(const EventBase& e, const Fn& fn, Fns... fns)
    {
        EventEater eater( fn );
        eater( e );

        eat( e, fns... );
    }
    template <typename... Fns>
    void eat(const EventBase& e)
    {
    }
};


inline CaseEvent caseOf(const EventList& es)
{
    return CaseEvent( es );
}

inline CaseEvent caseOf(const Event& e)
{
    return CaseEvent( e ); 
}


} // namespace batb


#endif

