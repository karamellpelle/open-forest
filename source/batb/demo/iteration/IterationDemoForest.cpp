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
#include "batb/forest/World.hpp"
#include "batb/value/forest.hpp"
#include "batb/value/run.hpp"
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
    
    run::World& run = demo.run;
    forest::World& forest = demo.forest;

    // set ticks to current run-tick
    demo.tick = run.tick;
    forest.tick = demo.tick;

    
    ////////////////////////////////////////////////////////////////////////////////
    // add some control's

    constexpr uint n = 8;
    constexpr float_t spread = 300.0;
    for (uint i = 0; i != n; ++i)
    {
        static std::default_random_engine rand; 
        std::uniform_int_distribution<int> gen_x( -spread, spread );
        std::uniform_int_distribution<int> gen_y( -spread, spread );
       
        int x = gen_x( rand );
        int y = gen_x( rand );
        static forest::ControlDefinition::Code code = 0;

        forest::ControlDefinition def( x, y, code++ );
        auto* control = forest.addControl( def );
    }

        
    ////////////////////////////////////////////////////////////////////////////////
    // add a Runner

    auto* runner = forest.addRunner();
    runner->reset( glm::vec2( 40, -120 ) );

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


