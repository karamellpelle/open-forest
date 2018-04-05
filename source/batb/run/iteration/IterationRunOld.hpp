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
#ifndef BATB_RUN_ITERATION_ITERATION_RUN_OLD_HPP
#define BATB_RUN_ITERATION_ITERATION_RUN_OLD_HPP
#include "batb/run/iteration/IterationRun.hpp"


namespace batb
{


class BATB;

namespace run
{




// IterationRunOld:
//    runs the old BATB code (this project is a fork of that), fully inside
//    the new BATB (open-forest)
//
class IterationRunOld : public IterationRun
{
friend void begin(IterationRunOld& );
friend void end(IterationRunOld& );

public:
    // construct from the containing Run object
    IterationRunOld(BATB& );

    // iterate
    IterationStack iterate_run(World& world) override;

    // setup before iteration
    void iterate_begin(World& );

    ////////////////////////////////////////


private:
};

////////////////////////////////////////////////////////////////////////////////
//  

void begin(IterationRunOld& );


void end(IterationRunOld& );



} // namespace run

} // namespace batb

#endif
