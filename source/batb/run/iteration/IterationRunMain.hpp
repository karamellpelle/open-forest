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
#ifndef BATB_RUN_ITERATION_ITERATION_RUN_MAIN_HPP
#define BATB_RUN_ITERATION_ITERATION_RUN_MAIN_HPP
#include "batb/run/iteration/IterationRun.hpp"
#include "batb/run/BeginEvents.hpp"


namespace batb
{
class BATB;


namespace run
{
class TBMain;



// the "main menu" (application is up and running)
class IterationRunMain : public IterationRun
{
public:
    IterationRunMain(BATB* );

    // setup
    void begin();
    void end();

    IterationStack iterate_run(World& world) override;

    void iterate_begin(World& );


    TBMain* tb_main = nullptr; 

private:
    BeginEvents beginEvents;


};

////////////////////////////////////////////////////////////////////////////////
//  


} // namespace run

} // namespace batb

#endif
