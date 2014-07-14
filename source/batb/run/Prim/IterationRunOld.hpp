//    orienteering-game: an orientering game.
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
#ifndef BATB_ITERATION_RUN_OLD_HPP
#define BATB_ITERATION_RUN_OLD_HPP
#include "BATB/Run/Prim/Iteration.hpp"
#include "BATB/Config.hpp"


namespace BATB
{


// iterate-iteration
class IterationRunOld1 : IterationRunWorld
{
friend class IterationRunOld0;

public:
    void iterate(IterationStackRunWorld& stack, RunWorld& run);

private:
    void create(xml::XMLElement* cfg);
    void destroy();
};



// begin-iteration 
class IterationRunOld0 : public IterationBeginnerRun
{
public:
    IterationRunOld0(IterationRunOld1* iter1) : IterationBeginnerRun( iter1 ) { }

    static IterationRunOld0*    create(xml::XMLElement* );
    static void                 destroy(IterationRunOld0* iter);


protected:
    void world_begin(RunWorld& );

    void destroy();
};





// externals are only referring to the starting iteration:
typedef IterationRunOld0 IterationRunOld;

}

#endif