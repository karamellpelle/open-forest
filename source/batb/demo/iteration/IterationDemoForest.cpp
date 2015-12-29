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
#include <iomanip>
//#include "helpers/bezier.hpp"

namespace batb
{

namespace demo
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationDemoForest::IterationDemoForest(BATB& b) : 
    IterationDemo( b ), outputDemo( b ), beginEventsDemo( b ), output( b ), beginEvents( b ),
    modifyControlCamera( b ), modifyControlRunner( b ), stepDT( b )
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

    // clear keys
    batb.demo.keyset.reset();
    batb.forest.keyset.reset();

    // no cursor
    batb.keys.setCursorFree( true );

    
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
        static forest::ControlDefinition::Code code = 50000;

        forest::ControlDefinition def( x, y, code++ );
        auto* control = forest.addControl( def );
    }

        
    ////////////////////////////////////////////////////////////////////////////////
    // add a Runner

    demo.runner = forest.addRunner();
    demo.runner->reset( glm::vec2( 40, -120 ) );

}


IterationStack IterationDemoForest::iterate_demo(World& demo)
{
    auto& forest = demo.forest;

    ////////////////////////////////////////////////////////////////////////////////
    // OUTPUT
    //

    // output forest
    output( forest );

    outputDemo( demo );

    

    ////////////////////////////////////////////////////////////////////////////////
    // STEP
    //

    // clean up and retrieve events outside of World
    beginEventsDemo( demo );     // demo::World
    beginEvents( forest ); // forest::World
    // run::Events ignored!


    ////////////////////////////////////////////////////////////////////////////////
    // TMP: move runner
    constexpr float_t radius = 150.0;
    float_t x, z;
    cossin( 0.5 * forest.tick, x, z );
    auto& pos = demo.runner->move.aim[3];
    pos.x = radius * x;
    pos.z = radius * z;

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

    // set new controls
    if ( batb.demo.keyset.new_course->click() )
    {
        createCourse( demo );
         
    }

    ////////////////////////////////////////////////////////////////////////////////
    // THINK
    //
    // TODO: look at demo-events!
    if ( batb.run.keyset.ogre->click() )
    {
        batb.log << "out of IterationRunDemo!!" << std::endl;

        // cursor
        batb.keys.setCursorFree( false );

        return _emptylist_;
    }
    else
    {
        return { this };
    }


}

void IterationDemoForest::createCourse(demo::World& demo)
{
    forest::World& forest = demo.forest;


    glm::vec4 p0;
    glm::vec4 p1;
    if ( 2 <= demo.course.size() )
    {
        // continue previous course 
        auto j = std::rbegin( demo.course );
        p1 = (*j)->aim[3];
        
        ++j;
        p0 = (*j)->aim[3];
    }
    else
    {
        // start new course
        p0 = glm::vec4( -1.0, 0.0, 0.0, 1.0);
        p1 = glm::vec4( 0.0, 0.0, 0.0, 1.0);

    }

    float_t w_min = p1.x;
    float_t w_max = p1.x;
    float_t h_min = p1.z;
    float_t h_max = p1.z;

    // remove old
    for ( auto* control : demo.course )
    {
        forest.removeControl( control );
    }
    demo.course.clear();

    using ControlDefinition = forest::ControlDefinition;   
    ControlDefinition::Code code = 0;

    auto addControl = [&](float_t x, float_t z, ControlDefinition::Code code, ControlDefinition::Type type)
    {
        ControlDefinition def( x, z, code );
        def.type = type;
        demo.course.push_back( forest.addControl( def ) );

        w_min = std::min( w_min, x );
        w_max = std::max( w_max, x );
        h_min = std::min( h_min, z );
        h_max = std::max( h_max, z );
    };
    
    // start
    addControl( p1.x, p1.z, code++, ControlDefinition::Type::Start );

    static std::default_random_engine rand; 

    constexpr uint max_controls = 8;
    std::uniform_int_distribution<uint> gen_m( 1, max_controls );
    uint m = gen_m( rand );

    for (uint i = 0; i != m; ++i)
    {
        glm::vec4 u = glm::normalize( p1 - p0 );
        auto trans = glm::mat4( u.x, 0.0, u.z, 0.0,
                                0.0, 0.0, 0.0, 0.0,
                                -u.z, 0.0, u.x, 0.0, 
                                p1.x, 0.0, p1.z, 1.0 );
     
        constexpr float_t spread_x0 = 300;
        constexpr float_t spread_x1 = 600;
        constexpr float_t spread_y = 1000;
        std::uniform_real_distribution<float_t> gen_x( spread_x0, spread_x1 );
        std::uniform_real_distribution<float_t> gen_y( -spread_y, spread_y );

        constexpr float_t s = 1.0;
        float_t x = s * gen_x( rand );
        float_t y = s * gen_y( rand );
        
        auto p2 = trans * glm::vec4( x, 0.0, y, 1.0 );

        if ( i + 1 == m )
        {
            code = std::max( (uint)(500), (uint)(code + 32) );
            addControl( p2.x, p2.z, code, ControlDefinition::Type::Finish );
        }
        else
        {
            // add a random code number
            constexpr uint delta = 23;
            uint code_d = std::uniform_int_distribution<uint>( 1, delta )( rand );
            code += code_d;
        
            addControl( p2.x, p2.z, code, ControlDefinition::Type::Normal );
        }


        p0 = p1;
        p1 = p2;
    }

    demo.course_dim = std::max( w_max - w_min, h_max - h_min );

}

////////////////////////////////////////////////////////////////////////////////
//
//

////////////////////////////////////////////////////////////////////////////////
// 
//



} // namespace demo

} // namespace batb


