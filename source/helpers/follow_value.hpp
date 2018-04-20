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
#ifndef HELPERS_FOLLOW_VALUE_HPP
#define HELPERS_FOLLOW_VALUE_HPP
#include "include.hpp"


// use time to follow a numeric value
template <typename T>
class follow_value
{
public:
    follow_value() : ideal(), current()                      { }
    follow_value(T t) : ideal( t ), current( (double)( t ) ) { }
   
    void  clear();
    void  clear(T );
    T     get() const;
    void  speed(double );
    void  step(double );

    // TODO: reset(double tick)
    // 

    T ideal         = T();
    double current  = T();

private:

    double tick_  = 0.0;    // tick
    double vel_   = 1.0;    // speed of movement
};


////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void follow_value<T>::clear()
{
    clear( ideal );
}

template <typename T>
inline void follow_value<T>::clear(T id)
{
    ideal = id;
    current = (double)( ideal );
}

template <typename T>
inline T follow_value<T>::get() const
{
    return (T)( current );
}

template <typename T>
inline void follow_value<T>::speed(double v)
{
    vel_ = std::max( 0.0, v );
}

template <typename T>
inline void follow_value<T>::step(double t)
{
    // current moves to ideal by speed and time

    auto i = (double)( ideal );

    // are we below?
    if ( current <= i )
    {

        current += vel_ * (t - tick_ );

        // dont move higher than ideal
        if ( i <= current  ) current = i;
    }
    // are we above?
    if ( i <= current )
    {

        current -= vel_ * (t - tick_ );

        // dont move lover than ideal
        if ( current <= i ) current = i;
    }

    tick_ = t;
}


#endif

