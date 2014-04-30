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
#ifndef BATB_ITERATION_RUN_BEGIN_HPP
#define BATB_ITERATION_RUN_BEGIN_HPP
#include "BATB/BATB_include.hpp"
#include "BATB/Run/Prim/Iteration.hpp"


namespace BATB
{


// start 
class IterationRunBegin0 : public IterationRunWorld
{
public:
    static IterationRunBegin0* create(Config::Block* cfg)
    static void destroy(IterationRunBegin0* iter);

    void iterate(IterationStackRunWorld& stack, RunWorld& run);           // to be run once, then calling IterationRunBegin1

private:
    IterationRunBegin1* next_;
};



// main-iteration
class IterationRunBegin1 : public IterationDefault<RunWorld>
{
friend class IterationRunBegin0;

public:
    static IterationRunBegin1* create(Config::Block* cfg);

    void iterate(IterationStackRunWorld& stack, RunWorld& run);   // FIXME: define in .cpp

private:
};



// externals are only referring to the starting iteration:
typedef IterationRunBegin0 IterationRunBegin;

}

#endif
