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
#ifndef BATB_RUN_ITERATION_ITERATION_RUN_BEGIN_HPP
#define BATB_RUN_ITERATION_ITERATION_RUN_BEGIN_HPP
#include "batb/run/iteration/IterationRun.hpp"
#include "batb/load/ThreadGLContext.hpp"
#include "batb/load/FiniteLoad.hpp"


namespace batb
{


class BATB;

namespace run
{




// IterationRunBegin:
//    loads the non-core part of BATB (and afterwards run::World??), 
//    showing the load progress interactively. finishes when load error
//    or load complete. continues with IterationRunMain if success loading...
class IterationRunBegin : public IterationRun
{
friend void begin(IterationRunBegin& );
friend void end(IterationRunBegin& );

public:
    // construct from the containing Run object
    IterationRunBegin(BATB& );

    // iterate
    IterationStack iterate_run(World& world) override;

    // setup before iteration
    void iterate_begin(World& );

    ////////////////////////////////////////


private:

    class Loader : public ThreadGLContext<FiniteLoad>
    {
    public: 
        Loader(BATB& b) : batb( b ) { }
        void run() override;
    private: 
        BATB& batb;
    } loader_;
};

////////////////////////////////////////////////////////////////////////////////
//  

void begin(IterationRunBegin& );


void end(IterationRunBegin& );



} // namespace run

} // namespace batb

#endif
