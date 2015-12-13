//    open-demo: an orientering game.
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
#include "batb.hpp"
#include "batb/demo/iteration/IterationDemoForest.hpp"
#include "batb/demo/World.hpp"
#include <random>
//#include "helpers/bezier.hpp"

namespace batb
{

namespace demo
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationDemoForest::IterationDemoForest(BATB& b) : 
    IterationDemo( b ), output( b ), modifyControlCamera( b ), modifyControlRunner( b ), stepDT( b )
{

}



void IterationDemoForest::iterate_begin(World& demo)
{
    BATB_LOG_FUNC( batb );

    // set ticks to current run-tick
    demo.tick = demo.run.tick;
    demo.forest.tick = demo.tick;
}


IterationStack IterationDemoForest::iterate_demo(World& demo)
{
    auto& forest = demo.forest;

    ////////////////////////////////////////////////////////////////////////////////
    // OUTPUT
    //

    // output forest
    output( forest );


    ////////////////////////////////////////////////////////////////////////////////
    // STEP
    //


    // control Camera
    modifyControlCamera( forest );

    // controlRunner
    modifyControlRunner( forest );


    tick_t tick_next = demo.tick;
    // prevent too many dt steps:
    forest.tick = forest.tick + value::dt_max <= tick_next ? tick_next - value::dt_max : forest.tick;

    // make a dt-step of forest::World
    while ( forest.tick + value::dt <= tick_next )
    {
      
        // step World
        stepDT( forest, value::dt );

        forest.tick += value::dt;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // THINK
    //
    // TODO: look at demo-events!
    if ( batb.run.keyset.ogre->click() )
    {
        batb.log << "out of IterationRunDemo!!" << std::endl;
        return _;
    }
    else
    {
        return { this };
    }


}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationDemoForest& iter)
{
    BATB_LOG_FUNC( iter.batb );


}

void end(IterationDemoForest& iter)
{
    BATB_LOG_FUNC( iter.batb );
}

////////////////////////////////////////////////////////////////////////////////
// 
//



} // namespace demo

} // namespace batb


