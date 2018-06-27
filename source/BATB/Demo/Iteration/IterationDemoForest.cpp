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
#include "BATB/OGRE.hpp"
#include "BATB/Value/Forest.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/glm.hpp"
#include "BATB/Screen.hpp"
#include "OgreRenderWindow.h"
#include <random>
#include <chrono>
#include <iomanip>


namespace batb
{

namespace demo
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationDemoForest::IterationDemoForest(BATB* b) : IterationDemo( b )
{

}



void IterationDemoForest::iterate_begin(World& demo)
{

    auto& run = *demo.run;
    auto& forest = *demo.forest;

    // set ticks to current run-tick
    demo.tick = run.tick;
    forest.tick = demo.tick;

    // clear keys
    batb->demo->keys->reset();
    batb->forest->keys->reset();

    // no cursor
    batb->keys->setCursorFree( true );

    // ensure we can use Key's
    batb->gui->lockKeys( false );


    ////////////////////////////////////////////////////////////////////////////////
    // add a runner, if first time

    if ( !demo.runner )
    {
        demo.runner = forest.addRunner( run.player ); 
        demo.runner->reset( glm::vec2( 0, 0 ) );
       //demo.runner->headlamp( true );

       // runner iff started before
       demo.aim_keyscontroller.connect( &demo.forest_drawer );
       demo.forest_drawer.cameraFree();
    }

    // if we have no Course, create one
    if ( demo.course.empty() )
    {
        ////////////////////////////////////////////////////////////////////////////////
        // create a course for runner
        createRandomCourse( demo ); 
        demo.course_i = 0;

        ////////////////////////////////////////////////////////////////////////////////
        // create a curve: control0 -> control1
        auto* control0 = demo.course[ demo.course_i ];
        auto* control1 = demo.course[ demo.course_i + 1 ];
        
        curve.create( glm::vec2( control0->aim.pos.x, control0->aim.pos.z ),
                      glm::vec2( control1->aim.pos.x, control1->aim.pos.z ) );
        curve_i = 0; 


    }

}


IterationStack IterationDemoForest::iterate_demo(World& demo)
{
    auto& forest = *(demo.forest);
    auto tick = demo.tick;

    ////////////////////////////////////////////////////////////////
    // since we are not running IterationForest-Iteration's directly:
    // update events (Forest)
    forest.events.step();

    // transfer events from Forest into forest::World
    forest.events.take( *batb->forest->events );


    ////////////////////////////////////////////////////////////////
    // output 
    output( demo );

    ////////////////////////////////////////////////////////////////
    // step
    return step( demo );

}


void IterationDemoForest::output(World& demo)
{
    auto& run    = *demo.run;
    auto& forest = *demo.forest;
    auto& scene  = run.scene;


    // TODO: use Scene of run::World

    ////////////////////////////////////////////////////////////////
    // 3D

    // draw forest::World 
    batb->ogre->sceneBegin( scene );
    demo.forest_drawer.draw( batb->ogre->ogre_renderwindow  );
    batb->ogre->sceneEnd();

    // AL (3D listener, effects, etc)
    auto aim = demo.forest_drawer.getAim();
    demo.sound.play3D( aim );


    ////////////////////////////////////////////////////////////////
    // 2D

    // draw map as overlay on Scene 
    //
    // we want to work on the FBO target with meter coordinates to make the 
    // map as real as possible.
    //
    auto nvg = batb->gl->nanovgBegin( scene );

    nvgSave( nvg );

    // define size of NanoVG context, in pixels and meters
    DemoMapDrawer::Draw draw;
    draw.wth   = scene.wth_px;
    draw.hth   = scene.hth_px;
    draw.wth_m = scene.wth_m;
    draw.hth_m = scene.hth_m;
    
    // set origo in the middle 
    nvgTranslate( nvg, 0.5 * scene.wth_px, 0.5 * scene.hth_px );

    //demo.map_drawer.setZoom(2.5);
    demo.map_drawer.mapscale( 1, 10000 );
    demo.map_drawer.useMap( nullptr ); 
    demo.map_drawer.draw( nvg, draw );

    nvgRestore( nvg );
    
    batb->gl->nanovgEnd(); 



    // background/foreground sound (music, etc.)
    demo.sound.play2D();


}

IterationStack IterationDemoForest::step(World& demo)
{
    auto& forest = *demo.forest;
    auto tick = demo.tick;
    ////////////////////////////////////////////////////////////////////////////////
    // control objects

    // "AI" for demo
    modifyRunnerDemo( demo );

    // use Keys to control aiming in forest::World, typically camera
    demo.aim_keyscontroller.step( batb );

    ////////////////////////////////////////////////////////////////////////////////
    // step physics (adds events)

    // make sure make sure we don't make a lot of dt steps (prevent hang)
    forest.tick = forest.tick + value::dt_max <= tick ?  tick - value::dt_max : forest.tick;

    // make as many dt-step of forest::World as possible since last frame
    while ( forest.tick + value::dt <= tick )
    {

        // step World (do)
        demo.step_dt.add( forest, value::dt );
       
        // look at events (think)
        for ( auto& ev : forest.events )
        {
            if ( auto e = eat<forest::event::ProximityControl>( ev ) )
            {
                //std::cout << std::setprecision( 2 ) << std::fixed << "\r"
                //          << "proximity of control " << e->control->definition.code << ": "
                //          << std::sqrt(e->epseps)
                //          ;

                // punch if close enough to control
                constexpr float_t punch_d = 20.0;
                if ( e->epseps < punch_d * punch_d )
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



                // if correct control punched, set next
                if ( code == code1 )
                {
                    // set next control in course
                    ++demo.course_i;

                    if ( demo.course_i + 1 == demo.course.size() )
                    {
                        // course complete, create new course
                        createRandomCourse( demo );
                        
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

    }

    // update states
    forest.update();
    
    // step output state
    demo.forest_drawer.step( forest.tick );
    demo.sound.step( forest.tick );


    // this iteratein runs forever 
    return { this };
    
}

// "AI" in our demo
void IterationDemoForest::modifyRunnerDemo(demo::World& demo)
{
    constexpr uint m = 16;

    auto& runner = demo.runner;

    if ( runner )
    {

        ////////////////////////////////////////////////////////////////////////////////
        auto p = glm::vec2( runner->move.pos.x, runner->move.pos.z );
        auto p1 = curve( m, curve_i );


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
                p1 = curve( m, curve_i + 1);

            }

        }


        // anyway, always move p against p1
        if ( p1 != p )
        {
            runner->setDirection( p1 - p );
            runner->setSpeed( 1.0 );
        }

    } 

}


////////////////////////////////////////////////////////////////////////////////
//
//
void IterationDemoForest::createRandomCourse(demo::World& demo)
{
    using ControlDefinition = forest::ControlDefinition;   
    auto& forest = *demo.forest;

    glm::vec4 p0;
    glm::vec4 p1;
    if ( 2 <= demo.course.size() )
    {
        auto size = demo.course.size();

        // continue previous course 
        p0 = demo.course[ size - 2 ]->aim.pos;
        p1 = demo.course[ size - 1 ]->aim.pos;
    }
    else
    {
        // start new course
        p0 = glm::vec4( 0.0, 0.0, -1.0, 1.0);
        p1 = glm::vec4( 0.0, 0.0, 0.0, 1.0);

    }

    // clear course
    // FIXME: no remove?
    for (uint i = 0; i != demo.course.size(); ++i)
    {
        forest.removeControl( demo.course[i] );
    }
    demo.course.clear();


    ControlDefinition::Code code = 0;

    // Start
    demo.course.addControl( p1.x, p1.z, code++, ControlDefinition::Type::Start );

    static std::default_random_engine rand( std::chrono::system_clock::now().time_since_epoch().count() );

    constexpr uint max_controls = 1;
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
            demo.course.addControl( p2.x, p2.z, code, ControlDefinition::Type::Finish );
        }
        else
        {
            // random code number
            uint code_d = std::uniform_int_distribution<uint>( 1, 23 )( rand );
            code += code_d;
            
            // Normal
            demo.course.addControl( p2.x, p2.z, code, ControlDefinition::Type::Normal );
        }


        p0 = p1;
        p1 = p2;
    }

}

////////////////////////////////////////////////////////////////////////////////
// 
//



} // namespace demo

} // namespace batb


