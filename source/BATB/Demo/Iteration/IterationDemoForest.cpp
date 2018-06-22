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


IterationDemoForest::IterationDemoForest(BATB* b) : 
    IterationDemo( b ), 
    modifyCamera( b ), 
    modifyRunner( b ),
    modifyControlCamera( b ), 
    modifyControlRunner( b ), stepDT( b )
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


        ////////////////////////////////////////////////////////////////////////////////
        // set controllers (Keys)
        modifyRunner.runner( demo.runner );
        modifyControlRunner.modifier( &modifyRunner );

        // move camera by ModifyControlCamera (manual movement)
        modifyControlCamera.modifier( &modifyCamera );
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

    // output 
    output( demo );

    // step
    return step( demo );

}


void IterationDemoForest::output(World& demo)
{
    auto& run    = *demo.run;
    auto& forest = *demo.forest;


    // TODO: use Scene of run::World

    ////////////////////////////////////////////////////////////////
    // 3D

    // draw forest::World 
    batb->ogre->sceneBegin( run.scene );
    demo.forest_drawer.draw( batb->ogre->ogre_renderwindow  );
    batb->ogre->sceneEnd();

    // TODO: ALURE (3D listener, effects, etc)


    ////////////////////////////////////////////////////////////////
    // 2D

    // draw map as overlay on Scene 
    //
    // we want to work on the FBO target with millimeter coordinates to make the 
    // map as real as possible.
    // we could assume 1 dot is 1/96inch, but that would not work with
    // a Scene/FBO representing the screen of a Retina display. 
    // the best would be (if FBO is a screen!) to assume that the Scene/FBO 
    // always has a typical size of for example 280 mm width.
    //
    // since we currently know that the Scene/FBO represents the screen, let's 
    // try to be precise and use specific tools to retrieve the size of the
    // Scene/FBO in millimeters
    //
    // TODO: implement methods for Scene to retrieve physical size (like below,
    //       when Scene represents the screen), or assume a certain mapping between
    //       pixels and millimeters (typically: 1 dot = 96 inch) (which is problematic
    //       for Retina screens).

    // see: http://www.glfw.org/docs/latest/monitor_guide.html
#if 1


    // create NanoVG context in meter coordinates
    auto nvg = batb->gl->nanovgBegin( run.scene );

    // scale from meters into pixels
    auto glfw_monitor = glfwGetPrimaryMonitor();
    auto mode = glfwGetVideoMode( glfw_monitor );
    int mw_mm, mh_mm;
    glfwGetMonitorPhysicalSize( glfw_monitor, &mw_mm, &mh_mm );
    int wth_win, hth_win;
    glfwGetWindowSize( batb->screen->glfw_window, &wth_win, &hth_win );
    // size of FBO, in meters
    double wth_m = (double)(mw_mm * wth_win) / (double)( 1000 * mode->width );
    double hth_m = (double)(mh_mm * hth_win) / (double)( 1000 * mode->height );

    nvgSave( nvg );

    // set origo in the middle 
    nvgTranslate( nvg, 0.5 * run.scene.wth, 0.5 * run.scene.hth );

    // define size of NanoVG context, in pixels and meters
    DemoMapDrawer::Draw draw;
    draw.wth = wth_win;
    draw.hth = hth_win;
    draw.wth_m = wth_m;
    draw.hth_m = hth_m;
    //draw.from_m = (1000.0 * (double)( mode->width ) ) / (double)( mw_mm );
    
/*
    auto nanovg_font_ = nvgFindFont( nvg, "CourseDrawer" );
    if ( nanovg_font_ == -1 )
    {
        // create font
        nanovg_font_ =  nvgCreateFont( nvg, "CourseDrawer", file::static_data( "BATB/CourseDrawer.ttf" ).c_str() );
    }
        nvgFillColor( nvg, nvgRGBA( 22, 213, 23, 255) );
	nvgFontSize(nvg, 500.0f / scale);
                nvgFontFaceId( nvg, nanovg_font_ );
                nvgText( nvg, 0, 0, std::to_string( 444 ).c_str(), nullptr );
*/


    //demo.map_drawer.setZoom(2.5);
    //demo.map_drawer.setRotation( 0.1 * demo.tick );
    //demo.map_drawer.setRotation( 0.1 );
    demo.map_drawer.mapscale( 1, 10000 );
    demo.map_drawer.useMap( nullptr ); 
    demo.map_drawer.draw( nvg, draw );

    //nvgStrokeWidth( nvg, 0.02 ); // 3 mm
    //// x
    //nvgStrokeColor(nvg, nvgRGBA(0,192,255,255));
    //nvgLineCap(nvg, NVG_ROUND );
    //nvgBeginPath( nvg );
    //nvgMoveTo( nvg, 0.00, 0.00 );
    //nvgLineTo( nvg, 0.00, 0.04 ); 
    //nvgStroke( nvg );

    nvgRestore( nvg );
    
    batb->gl->nanovgEnd(); 
#endif
    // TODO: ALURE: background sound (music)

    // TODO: output

}

IterationStack IterationDemoForest::step(World& demo)
{
    auto& forest = *demo.forest;
    auto tick = demo.tick;
    ////////////////////////////////////////////////////////////////////////////////
    // control objects

    // "AI" for demo
    modifyRunnerDemo( demo );

    // use Keys to control objects in forest::World
    modifyControlCamera( forest, tick );
    modifyControlRunner( forest );

  
    ////////////////////////////////////////////////////////////////////////////////
    // movement
    modifyCamera( forest );
    modifyRunner( forest );

  
    ////////////////////////////////////////////////////////////////////////////////
    // step physics (adds events)

    forest.tick = forest.tick + value::dt_max <= tick ? // prevent too many dt steps
                  tick - value::dt_max : forest.tick;

    // make a dt-step of forest::World
    while ( forest.tick + value::dt <= tick )
    {
        // step World
        stepDT( forest, value::dt );
       
        // look at events (think)
        for ( auto& ev : forest.events )
        {
            if ( auto e = eat<forest::event::ProximityControl>( ev ) )
            {
                //std::cout << std::setprecision( 2 ) << std::fixed << "\r"
                //          << "proximity of control " << e->control->definition.code << ": "
                //          << eps
                //          ;

                // punch if close enough to control
                constexpr float_t punch_d = 25.0;
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

        // update tick of world
        forest.tick += value::dt;
    }
    // update after dt
    stepDT( forest );

    // this one runs forever 
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


