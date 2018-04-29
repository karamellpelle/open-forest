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
class IterationRunDemo;


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
    //IterationRunIntro     iterationRunIntro;           // fancy game inro. use video?
    //IterationRunOutro     iterationRunOutro;           // fancy game outro. scrolling text. credits, etc.
    std::unique_ptr<IterationRunMain> iterationRunMain;  // main menu
    std::unique_ptr<IterationRunOld>  iterationRunOld;   // run old BATB (may not be functional depending on build setting)
    std::unique_ptr<IterationRunDemo>  iterationRunDemo; // run old demo, i.e. forest::World trough demo::World through this world, run::World


    ////////////////////////////////////////////////////////////////////////////////
    // push events
    // TODO: shared_ptr
    template <typename T>
    void pushEvent(const T& d);
    template <typename T, typename D = std::default_delete<T>> // TODO enable_if T is_pointer
    void pushEvent(T* d, const D& del = D());

    // events to move over to world
    std::unique_ptr<EventList> events;

};

template <typename T>
inline void Run::pushEvent(const T& d)
{
    events->push( d );
}

template <typename T, typename D> 
inline void Run::pushEvent(T* d, const D& del)
{
    events->push( d, del );
}


} // namespace run

} // namespace batb

#endif

