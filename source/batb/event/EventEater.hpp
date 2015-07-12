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
#ifndef BATB_EVENT_EVENT_EATER_HPP
#define BATB_EVENT_EVENT_EATER_HPP
#include "batb/event/EventBase.hpp"
#include "batb/event/Event.hpp"
#include "batb/event/EventList.hpp"



namespace batb
{


////////////////////////////////////////////////////////////////////////////////
// EventEater
//
//

// EventEaterImpl
class EventEaterImpl
{
public:
    virtual bool eat(const EventBase& ) = 0; 
};



// EventEaterImplTyped
template <typename T>
class EventEaterImplTyped : public EventEaterImpl
{
friend class EventEater;

private:
    EventEaterImplTyped(const std::function<void (const T& )>& f) : function_( f ) { }

    virtual bool eat(const EventBase& data)
    {
        if ( std::type_index( typeid( T ) ) == data.type() )
        {
            T* hold  = static_cast<T*>( data.data() );

            // do actual call
            function_( *hold );

            return true;
        }
        return false;
    }

    std::function<void (const T& )> function_;
    

};


////////////////////////////////////////////////////////////////////////////////
// EventEater
//
class EventEater
{
friend class EventEaterSet;

public:
    template <typename T>
    EventEater(void (*f)(const T& )) : EventEater( std::function<void (const T& )>( f ) ) { }
    template <typename T>
    EventEater(const std::function<void (const T& )>& f) : impl_( new EventEaterImplTyped<T>( f ) ) { }
    template <typename T, class Class, class ClassObj>
    EventEater(void (Class::*method)(const T& ), ClassObj obj) : impl_( new EventEaterImplTyped<T>( std::bind( method, obj, std::placeholders::_1) ) ) { }


    bool operator()(const Event& event) const
    {
        return impl_->eat( *event.base_ );
    }

private:
    bool operator()(const EventBase& data) const
    {
        return impl_->eat( data );
    }

    std::shared_ptr<EventEaterImpl> impl_;

};


////////////////////////////////////////////////////////////////////////////////
// set of EventEater's
//

class EventEaterSet
{
public:
    void eat(const EventList& list)
    {
        for (auto& e : list.events_)
        {
            eat( *e );     
        }
    }
    void eat(const Event& e)
    {
        //for ( const auto& i = std::begin( eaters_ ); i != std::end( eaters_ ); ++i )
        for (const auto& eater : eaters_ )
        {
            eater( e );
        }
    }

    // push eater function
    template <typename T>
    void push(void (*f)(const T& ))
    {
        push( std::function<void(const T&)>( f ) );
    }
    // push eater function, object and member function
    template <typename T, class Class, class ClassObj>
    void push(void (Class::*method)(const T& ), ClassObj obj)
    {
        push( EventEater( method, obj ) );
    }
    // push eater function
    template <typename T>
    void push(const std::function<void(const T& )>& f)
    {
        push( EventEater( f ) );
    }

    // push eater
    void push(const EventEater& eater)
    {
        eaters_.push_back( eater );
    }

    // TODO: remove<T>()
    // clear

private:
    // (no use to be public; event is private and hidden)
    void eat(const EventBase& event)
    {
        for ( auto& eat : eaters_ )
        {
            eat( event );
        }
    }

    std::vector<EventEater> eaters_;
};


} // namespace batb


#endif

