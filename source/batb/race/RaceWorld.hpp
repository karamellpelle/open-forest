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
#ifndef BATB_RACE_RACEWORLD_HPP
#define BATB_RACE_RACEWORLD_HPP
#include "BATB/Race/RaceClass.hpp"
#include "BATB/Forest.hpp"

namespace BATB
{

class RunWorld;

class RaceWorld
{
public:
    typedef std::forward_list<RaceClass>  RaceClassList;

    RaceWorld(RunWorld* r) : forest( r ), run_( r )    // worlds shall not be singletons!
    {
    }


    // all classes for this race 
    RaceClassList classes;

    // forest 
    ForestWorld forest;
    IterationStackForestWorld forest_stack;

    RunWorld* runworld()
    {
        return run_;
    }

private:
    // RaceWorld implies a RunWorld, the RunWorld containing 'this'.
    // it would be nice to let RaceWorld be totally independent of RunWorld,
    // so that RaceWorld works alone. however, RaceWorld needs access to
    // external resources. for exapmle Player's, containing information for each
    // Runner in RaceWorld. we let such external resources be part of RunWorld.
    // also, since there may (in theory) be more than one RunWorld, some of these
    // external resources could be different for two different RunWorld's.
    RunWorld* const run_;

    // meta:
    std::string name;
    
};


}

#endif
