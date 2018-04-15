//    open-demo: an orientering game.
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
#include "BATB/Run/World.hpp"
#include "BATB/Run.hpp"
#include "BATB/Run/KeySet.hpp"
#include "BATB/Demo.hpp"
#include "BATB/Demo/World.hpp"
#include "BATB/Demo/Iteration/IterationDemoForest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/events.hpp"
#include "BATB/GUI.hpp"
#include "BATB/AL.hpp"
#include "BATB/Value/Forest.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/glm.hpp"
#include <random>
#include <chrono>
#include <iomanip>


namespace batb
{

namespace demo
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationDemoForest::IterationDemoForest(BATB* b) : 
    IterationDemo( b ), outputDemo( b ), beginEventsDemo( b ), output( b ), beginEvents( b ),
    modifyCamera( b ), modifyRunner( b ),
    modifyControlCamera( b ), modifyControlRunner( b ), stepDT( b )
{

}



void IterationDemoForest::iterate_begin(World& demo)
{
    
    run::World& run = demo.run;
    forest::World& forest = demo.forest;

    // set ticks to current run-tick
    demo.tick = run.tick;
    forest.tick = demo.tick;

    // clear keys
    batb->demo->keyset->reset();
    batb->forest->keyset->reset();

    // no cursor
    batb->keys->setCursorFree( true );

    
    ////////////////////////////////////////////////////////////////////////////////
    // add a runner
    demo.runner = forest.addRunner( run.player );
    demo.runner->reset( glm::vec2( 0, 0 ) );


    ////////////////////////////////////////////////////////////////////////////////
    // create a course for runner
    createCourse( demo );
    demo.course_i = 0;

    ////////////////////////////////////////////////////////////////////////////////
    // create a curve: control0 -> control1
    auto* control0 = demo.course[ demo.course_i ];
    auto* control1 = demo.course[ demo.course_i + 1 ];
    
    curve.create( glm::vec2( control0->aim.pos.x, control0->aim.pos.z ),
                  glm::vec2( control1->aim.pos.x, control1->aim.pos.z ) );
    curve_i = 0;


    ////////////////////////////////////////////////////////////////////////////////
    // set controllers (Keys)
    modifyRunner.runner( demo.runner );
    modifyControlRunner.modifier( &modifyRunner );
    modifyControlCamera.modifier( &modifyCamera );

    // ensure we can use Key's
    batb->gui->lockKeys( false );

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
    // (we ignore run::Events in IterationDemoForest)
    
    
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
    forest.tick = forest.tick + value::dt_max <= tick_next ? // prevent too many dt steps
                  tick_next - value::dt_max : forest.tick;

    // make a dt-step of forest::World
    while ( forest.tick + value::dt <= tick_next )
    {
        // step World
        stepDT( forest, value::dt );
       
        // look at events (think)
        for ( auto& ev : forest.events )
        {
            if ( auto e = eat<forest::event::ProximityControl>( ev ) )
            {
                auto eps = std::sqrt( e->epseps );

                //std::cout << std::setprecision( 2 ) << std::fixed << "\r"
                //          << "proximity of control " << e->control->definition.code << ": "
                //          << eps
                //          ;

                // punch if close enough to control
                constexpr float_t punch_d = 25.0;
                if ( eps < punch_d )
                {
                    if ( e->runner->control0 != e->control && 
                         e->control->definition.type != forest::ControlDefinition::Type::Start )
                    {
                        e->runner->punch( e->control );
                    }
                }
            }
            if ( auto e = eat<forest::event::ControlPunch>( ev ) )
            {
                auto code = e->control->definition.code;
                auto code1 = demo.course[ demo.course_i + 1 ]->definition.code;

                // outpu sound
                // TODO: use dedicated alure::Buffer loaded by forest::World
                // create a source aimed upwards 
                glm::mat4 source_aim;
                source_aim[3] = e->control->aim.pos;
                auto src = batb->al->source( file::static_data( "BATB/Forest/audio/si_punch.mp3" ), source_aim);

                // does not work for me!!!
                src.setDistanceRange( 1.0, 50.0 );
                src.setConeAngles( 60.0, 300.0 ) ;
                src.setRolloffFactors( 8.0 );
                src.setGain( 0.5 );

                //std::cout << "control: \n"
                //          << "x: " << source_aim[3].x << std::endl
                //          << "y: " << source_aim[3].y << std::endl
                //          << "z: " << source_aim[3].z << std::endl
                //          << "camera: \n"
                //          << "x: " << forest.camera.move.pos.x << std::endl
                //          << "y: " << forest.camera.move.pos.y << std::endl
                //          << "z: " << forest.camera.move.pos.z << std::endl
                //          << std::endl;


                // if correct control punched, set next
                if ( code == code1 )
                {
                    // set next control in course
                    ++demo.course_i;

                    if ( demo.course_i + 1 == demo.course.size() )
                    {
                        // course complete, create new course
                        createCourse( demo );
                        
                        demo.course_i = 0;
                    }

                    // create a curve: control0 -> control1
                    auto* control0 = demo.course[ demo.course_i ];
                    auto* control1 = demo.course[ demo.course_i + 1 ];
                    curve.create( glm::vec2( control0->aim.pos.x, control0->aim.pos.z ),
                                  glm::vec2( control1->aim.pos.x, control1->aim.pos.z ) );
                    curve_i = 0;
                }

            }
        }

        // update tick of world
        forest.tick += value::dt;
    }
    // update after dt
    stepDT( forest );


    // TODO: look at demo-events!
    if ( batb->run->keyset->escape->click() )
    {
        // set back cursor
        batb->keys->setCursorFree( false );

        return _emptylist_;
    }
    else
    {
        return { this };
    }


}


void IterationDemoForest::modifyRunnerDemo(demo::World& demo)
{
    constexpr uint m = 16;

    auto& runner = demo.runner;

    if ( runner )
    {
        ////////////////////////////////////////////////////////////////////////////////
        auto p = glm::vec2( runner->move.pos.x, runner->move.pos.z );
        auto p0 = curve( m, curve_i );
        auto p1 = curve( m, curve_i + 1 );


        // if runner at p1, run to p2
        constexpr float eps = 1.0;
        if ( inside( p1, eps, p ) )
        {
            if ( curve_i == m )
            {
                // (last point on the curve control0 -> control1 reached.
                // it is now up to the runner to punch control1!)
            }
            else
            {
                ++curve_i;

                // step curve
                p0 = curve( m, curve_i );
                p1 = curve( m, curve_i + 1);

            }

        }


        // anyway, always move p against p1
        if ( p1 != p )
        {
            modifyRunner.aim( p1 - p );
            modifyRunner.speed( 1.0 );
        }

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
        p0 = course[ size - 2 ]->aim.pos;
        p1 = course[ size - 1 ]->aim.pos;
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

    static std::default_random_engine rand( std::chrono::system_clock::now().time_since_epoch().count() );

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


