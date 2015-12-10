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
#ifndef BATB_RUN_ITERATION_ITERATION_RUN_MAIN_HPP
#define BATB_RUN_ITERATION_ITERATION_RUN_MAIN_HPP
#include "batb/run/iteration/IterationRun.hpp"
#include "batb/run/iteration/IterationRunMain/RunMainTBWidget.hpp"

// tmp
#include "batb/forest.hpp"

namespace batb
{


class BATB;

namespace run
{




// IterationRunMain:
//    application is loaded, up and running. present the main screen,
//    after IterationRunIntro (?). here the user starts to use our 
//    application, this is the "main menu".
//
class IterationRunMain : public IterationRun
{
friend void begin(IterationRunMain& );
friend void end(IterationRunMain& );

public:
    // construct from the containing BATB
    IterationRunMain(BATB& );

    // iterate
    IterationStack iterate_run(World& world) override;

    // setup before iteration
    void iterate_begin(World& );

    ////////////////////////////////////////

private:
    RunMainTBWidget* tb_widget_ = nullptr; // TODO?: general TBWidget, and this only pushes events?
    EventList events_;

    //// tmp: FIXME: into IterationRunDemo
    //forest::World* forest = nullptr;
    //forest::IterationStack forest_stack;
    EventEaterSet eater_set;
    void eat_number(const uint& n)
    {
        std::cout << "IterationRunMain eating number: " << n << std::endl;
    }

};

////////////////////////////////////////////////////////////////////////////////
//  

void begin(IterationRunMain& );


void end(IterationRunMain& );



} // namespace run

} // namespace batb

#endif
