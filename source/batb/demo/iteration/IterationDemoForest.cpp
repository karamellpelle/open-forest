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
#include "batb/glm.hpp"
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
    modifyCamera( b ), modifyRunner( b ),
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
    // add a runner
    demo.runner = forest.addRunner();
    demo.runner->reset( glm::vec2( 0, 0 ) );


    ////////////////////////////////////////////////////////////////////////////////
    // create a course for runner
    createCourse( demo );
    demo.course_i = 0;

    ////////////////////////////////////////////////////////////////////////////////
    // create a curve: control0 -> control1
    auto* control0 = demo.course[ demo.course_i ];
    auto* control1 = demo.course[ demo.course_i + 1 ];
    curve.create( glm::vec2( control0->aim[3].x, control0->aim[3].z ),
                  glm::vec2( control1->aim[3].x, control1->aim[3].z ) );
    curve_i = 0;


    ////////////////////////////////////////////////////////////////////////////////
    // set controllers (Keys)
    modifyRunner.runner( demo.runner );
    modifyControlRunner.modifier( &modifyRunner );
    modifyControlCamera.modifier( &modifyCamera );

}


IterationStack IterationDemoForest::iterate_demo(World& demo)
{
    auto& forest = demo.forest;

    ////////////////////////////////////////////////////////////////////////////////
    // *** output ***

    // output forest::World
    output( forest );

    // output demo::World
    outputDemo( demo );

    

    ////////////////////////////////////////////////////////////////////////////////
    // *** step ***

    ////////////////////////////////////////////////////////////////////////////////
    // clean up and retrieve events outside of World
    //
    beginEventsDemo( demo );      // demo::World
    beginEvents( forest );        // forest::World
    // we ignore run::Events

    
    ////////////////////////////////////////////////////////////////////////////////
    // control objects

    // "AI" for demo
    modifyRunnerDemo( demo );

    // use Keys to control objects in forest::World
    modifyControlCamera( forest );
    modifyControlRunner( forest );

  
    ////////////////////////////////////////////////////////////////////////////////
    // movement
    modifyCamera( forest );
    modifyRunner( forest );

  
    ////////////////////////////////////////////////////////////////////////////////
    // step physics (adds events)
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
    // think
    // TODO: look at demo-events!
    //
    if ( batb.run.keyset.escape->click() )
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


void IterationDemoForest::modifyRunnerDemo(demo::World& demo)
{
    constexpr uint m = 8;

    auto& runner = demo.runner;

    if ( runner )
    {
        ////////////////////////////////////////////////////////////////////////////////
        auto p = glm::vec2( runner->move.aim[3].x, runner->move.aim[3].z );
        auto p0 = curve( m, curve_i );
        auto p1 = curve( m, curve_i + 1 );


        // if runner at p1, run to p2
        constexpr float eps = 1.0;
        if ( inside( p1, eps, p ) )
        {
            // next control reached
            if ( ++curve_i == m ) // stop on last segment
            {
                ++demo.course_i;

                // TODO: remove from here and instead look at EventControl punch
                if ( demo.course_i + 1 == demo.course.size() )
                {
                    // course complete, create new course
                    createCourse( demo );
                    
                    demo.course_i = 0;
                }

                // control0 -> control1
                auto* control0 = demo.course[ demo.course_i ];
                auto* control1 = demo.course[ demo.course_i + 1 ];

                // create curve: control0 -> control1
                curve.create( glm::vec2( control0->aim[3].x, control0->aim[3].z ),
                              glm::vec2( control1->aim[3].x, control1->aim[3].z ) );

                curve_i = 0;
            }

            // set next curve
            p0 = curve( m, curve_i );
            p1 = curve( m, curve_i + 1);

        }


        // anyway, always move p against p1
        modifyRunner.aim( p1 - p );
        modifyRunner.speed( 1.0 );

    } 

}

void IterationDemoForest::createCourse(demo::World& demo)
{
    using ControlDefinition = forest::ControlDefinition;   
    forest::World& forest = demo.forest;
    Course& course = demo.course;

    glm::vec4 p0;
    glm::vec4 p1;
    if ( 2 <= course.size() )
    {
        auto size = course.size();

        // continue previous course 
        p0 = course[ size - 2 ]->aim[3];
        p1 = course[ size - 1 ]->aim[3];
    }
    else
    {
        // start new course
        p0 = glm::vec4( 0.0, 0.0, -1.0, 1.0);
        p1 = glm::vec4( 0.0, 0.0, 0.0, 1.0);

    }

    // clear course
    // FIXME: no remove?
    for (uint i = 0; i != course.size(); ++i)
    {
        forest.removeControl( course[i] );
    }
    course.clear();


    ControlDefinition::Code code = 0;

    // Start
    course.addControl( p1.x, p1.z, code++, ControlDefinition::Type::Start );

    static std::default_random_engine rand; 

    constexpr uint max_controls = 16;
    uint m = std::uniform_int_distribution<uint>( 1, max_controls )( rand );

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
        constexpr float_t s = 1.0;
        float_t x = s * std::uniform_real_distribution<float_t>( spread_x0, spread_x1 )( rand );
        float_t y = s * std::uniform_real_distribution<float_t>( -spread_y, spread_y )( rand );
        
        auto p2 = trans * glm::vec4( x, 0.0, y, 1.0 );

        if ( i + 1 == m )
        {
            // Finish
            code = std::max( (uint)(500), (uint)(code + 32) );
            course.addControl( p2.x, p2.z, code, ControlDefinition::Type::Finish );
        }
        else
        {
            // random code number
            uint code_d = std::uniform_int_distribution<uint>( 1, 23 )( rand );
            code += code_d;
            
            // Normal
            course.addControl( p2.x, p2.z, code, ControlDefinition::Type::Normal );
        }


        p0 = p1;
        p1 = p2;
    }

}

////////////////////////////////////////////////////////////////////////////////
//
//

////////////////////////////////////////////////////////////////////////////////
// 
//



} // namespace demo

} // namespace batb


