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
#ifndef BATB_RUN_HPP
#define BATB_RUN_HPP
#include "BATB/ModuleBATB.hpp"



namespace batb
{

class Event;


namespace run
{

class Console;
class Notify;
class KeySet;
class IterationRunMain;
class IterationRunOld;


class Run : public ModuleBATB
{
public:

    Run(BATB* );
    ~Run();

    // setup
    void begin(const std::string& );
    void end();


    // text interface (world modifier)
    std::unique_ptr<Console> console;
    // notify
    std::unique_ptr<Notify> notify;
    // set of key's
    std::unique_ptr<KeySet> keys;


    // Iteration's
    //IterationRunIntro     iterationRunIntro;
    std::unique_ptr<IterationRunMain> iterationRunMain;
    //IterationRunOutro     iterationRunOutro;
    std::unique_ptr<IterationRunOld>  iterationRunOld; 

    ////////////////////////////////////////////////////////////////////////////////
    // push events
    template <typename T>
    void pushEvent(const T& d)
    {
        events->push( d );
    }
    template <typename T, typename D = std::default_delete<T>> // enable_if is_pointer
    void pushEvent(T* d, const D& del = D())
    {
        events->push( d, del );
    }


    // events to move over to world
    std::unique_ptr<EventList> events;

};

} // namespace run

} // namespace batb

#endif

