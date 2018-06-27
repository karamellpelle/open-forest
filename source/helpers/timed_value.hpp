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
#ifndef HELPERS_TIMED_VALUE_HPP
#define HELPERS_TIMED_VALUE_HPP
#include "include.hpp"


// use time to reach a numeric value
template <typename T>
class timed_value
{
public:
    timed_value(T t) : ideal_( t )   { }
    timed_value(double tm, T t) : time_( tm ), ideal_( t )   { }
 
    void  time(double time);
    void  clear(double tick);

    // value depends on time
    T operator()(double tick) const;
    T operator()(double time, double tick) const;


private:
    double tick_  = 0.0;   // tick
    double time_  = 0.0;   // time to ideal reached
    T      ideal_ = T();
};


////////////////////////////////////////////////////////////////////////////////


template <typename T>
inline T timed_value<T>::operator()(double time, double tick) const
{
    double ticks = std::min( tick - tick_, time );
    double a = ticks / time;

    return (T)( (double)( ideal_ ) * a );
}

template <typename T>
inline T timed_value<T>::operator()(double tick) const
{
    return operator()( time_, tick );
}

template <typename T>
inline void  timed_value<T>::clear(double tick)
{
    tick_ = tick;
}

template <typename T>
inline void  timed_value<T>::time(double time)
{
    time_ = time;
}

#endif

