//    open-forest: an orientering game.
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
#ifndef BATB_RUN_RUN_HPP
#define BATB_RUN_RUN_HPP
#include "batb/ModuleBATB.hpp"
#include "batb/run/KeySet.hpp"
#include "batb/run/iteration/IterationRunMain.hpp"
#include "batb/run/iteration/IterationRunOld.hpp"




namespace batb
{




namespace run
{




class Run : public ModuleBATB
{
friend void begin(Run& );
friend void end(Run& );

public:
    Run(BATB& );

    KeySet keyset;

    // Iteration's
    //IterationRunIntro     iterationRunIntro;
    IterationRunMain      iterationRunMain;
    //IterationRunOutro     iterationRunOutro;
    IterationRunOld       iterationRunOld; 

    // the world 'this' is currently working on:
    // (working target for GUI, ...)
    World* world = nullptr;

private:

};


void begin(Run& );
void end(Run& );

} // namespace run

} // namespace batb

#endif

