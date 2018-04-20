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
#include <iomanip>
#include "BATB/CourseDrawer.hpp"
#include "BATB/Demo.hpp"
#include "BATB/Demo/Output.hpp"
#include "BATB/Demo/World.hpp"
#include "BATB/Run/World.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/GL.hpp"
#include "BATB/glm.hpp"


namespace batb
{

namespace demo
{

  
constexpr tick_t smooth_ticks = 1.5;
constexpr tick_t smooth_ticks_d = 0.8;

inline void nvg_point(NVGcontext* vg, float_t x, float_t y, float r = 14.0)
{
    nvgBeginPath(vg);
    nvgArc( vg, x, y, r, 0, twopi, NVG_CCW );
    nvgFill( vg );
    //nvgRoundedRect(vg, x - r, y - r, 2 * r, 2 * r, 0.5 * r );
    //nvgStroke( vg );
}

Output::Output(BATB* b) : batb( b )
{
    course_p0_ = glm::vec2( 0, 0 );
    course_p1_ = glm::vec2( 0, 0 );
    course_u0_ = glm::vec2( 1, 0 );
    course_u1_ = glm::vec2( 1, 0 );

}

void Output::operator()(World& demo)
{
    using namespace forest;
    auto& forest = demo.forest;

    if ( demo.runner )
    {
        const auto& trace = demo.runner->trace;
        auto& run = demo.run;
        auto pos = demo.runner->move.pos;
        auto& course = demo.course;

        // update view according to World
        update( demo );


        ////////////////////////////////////////////////////////////////////////////////
        // begin nanovg drawing
        auto* nvg = batb->gl->nanovgBegin( run.scene );

        nvgSave( nvg );

        ////////////////////////////////////////////////////////////////////////////////
        // setup map World space -> pixel space
        auto wth = demo.run.scene.wth;
        auto hth = demo.run.scene.hth;

        // set origo in the middle 
        nvgTranslate( nvg, wth / 2, hth / 2 );
       
        course_size_ = 150;

  
        ////////////////////////////////////////////////////////////////////////////////
        // set draw transfomation

        float_t alpha = course_tick_ + smooth_ticks <= run.tick ? 1.0 : (run.tick - course_tick_) / smooth_ticks;
        float_t alpha_d = course_tick_d_ + smooth_ticks_d <= run.tick ? 1.0 : (run.tick - course_tick_d_) / smooth_ticks_d;

        auto p = glm::mix( course_p0_, course_p1_, alpha );
        auto v = glm::mix( course_u0_, course_u1_, alpha );
        auto u = glm::length( v ) == 0 ? v : glm::normalize( v );
        auto d = smooth( course_d0_, course_d1_, alpha_d );


        // scaling
        float_t scale = (float_t)(hth) / ((d + 2.0 * course_size_) * 1.6 );
        nvgScale( nvg, scale, scale );

        // direct to -y axis of nanovg (up)
        nvgTransform( nvg, -u.y, -u.x,
                          u.x, -u.y,
                          0.0, 0.0 );

        // center 
        nvgTranslate( nvg, -p.x, -p.y );


        ////////////////////////////////////////////////////////////////////////////////
        // draw trace

        auto moveTo = [&](const TracePoint& p) { nvgMoveTo( nvg, p.x, p.z ); };
        auto lineTo = [&](const TracePoint& p) { nvgLineTo( nvg, p.x, p.z ); };

        nvgStrokeWidth( nvg, 12 );
        nvgLineCap(nvg, NVG_ROUND );
        nvgStrokeColor(nvg, nvgRGBA(0,192,255,128));

        const auto& points = trace.points;
        if ( !points.empty() )
        {
            nvgBeginPath(nvg);
            constexpr uint tails = 32; 
            //uint b = tails <= points.size() ? points.size() - tails : 0;
            uint b = 0;
            uint e = points.size();
            moveTo( points[b] );
            for (uint i = b + 1; i != points.size(); ++i)
            {
                lineTo( points[i] );
            }
            lineTo( trace.point0 );
            lineTo( TracePoint( forest.tick, glm::vec3( pos ) ) );

            nvgStroke(nvg);
            
        }

        // draw current position
        nvgStrokeColor(nvg, nvgRGBA(255,0,0,255));
        nvgFillColor(nvg, nvgRGBA(255,0,0,255));
        nvg_point( nvg, pos.x, pos.z ); 
        
        ////////////////////////////////////////////////////////////////////////////////
        // draw course

        if ( 2 <= course.size() )
        {
            CourseDrawer drawer( batb );
            drawer.numbers( true );
            drawer.size( course_size_ ); // 64

            drawer.begin();
            for (uint i = 0; i != course.size(); ++i)
            {
                forest::Control* control = course[i];
                auto pos = control->aim.pos;

                switch ( control->definition.type )
                {
                case forest::ControlDefinition::Type::Start:
                    drawer.start( glm::vec2( pos.x, pos.z ) );
                break;
                case forest::ControlDefinition::Type::Normal:
                    drawer.normal( glm::vec2( pos.x, pos.z ) );
                break;
                case forest::ControlDefinition::Type::Finish:
                    drawer.finish( glm::vec2( pos.x, pos.z ) );
                break;
                }


            }
            drawer.end();
        }

        nvgRestore( nvg );
        batb->gl->nanovgEnd();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // TODO: other output
    //  * sound 
    //  * network
    //  * 

}


void Output::update(World& demo)
{
    run::World& run = demo.run;

    // if new world, setup
    if ( world_ != &demo )
    {
        aim( demo );
        world_ = &demo;
    }

    ////////////////////////////////////////////////////////////////////////////////

    auto pos0 = demo.course[ demo.course_i ]->aim.pos;
    auto pos1 = demo.course[ demo.course_i + 1 ]->aim.pos;

    auto p0 = course_p1_;
    auto p1 = 0.5f * cast_xz( pos0 + pos1 );
  
    // change aim if new control
    if ( p0 != p1 )
    {
        aim( demo );
        course_i_ = demo.course_i;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // change d (zoom) if we want to view whole map

    if ( batb->demo->keys->map_view_full->pressed() )
    {
        // set 0
        float_t alpha_d = clamp(  (run.tick - course_tick_d_) / smooth_ticks_d, 0.0, 1.0 );
        course_d0_ = smooth( course_d0_, course_d1_, alpha_d );

        // set 1
        auto w = demo.course.width( course_p1_.x ) * demo.run.scene.shape.hth;
        auto h = demo.course.height( course_p1_.y );
        course_d1_ = std::max( course_d1_, (float_t)( 0.5 * std::max( w, h ) ) );


        course_tick_d_ = run.tick;

    }
    if ( batb->demo->keys->map_view_full->released() )
    {
        // set 0
        float_t alpha_d = clamp( (run.tick - course_tick_d_) / smooth_ticks_d, 0.0, 1.0 );
        course_d0_ = smooth( course_d0_, course_d1_, alpha_d );

        // set 1
        forest::Control* control0 = demo.course[ demo.course_i ];
        forest::Control* control1 = demo.course[ demo.course_i + 1 ];
        auto p0 = glm::vec2( control0->aim.pos.x, control0->aim.pos.z ); 
        auto p1 = glm::vec2( control1->aim.pos.x, control1->aim.pos.z ); 
        course_d1_ = glm::distance( p0, p1 );


        course_tick_d_ = run.tick;
    }


}


void Output::aim(World& demo)
{
    run::World& run = demo.run;

    // ignore setting d if zooming out
    bool map_view = batb->demo->keys->map_view_full->press();

    float_t alpha = clamp(   (run.tick - course_tick_) / smooth_ticks,  0.0,  1.0 );
    float_t alpha_d = clamp( (run.tick - course_tick_d_) / smooth_ticks, 0.0, 1.0 ); // here: use 'smooth_ticks', not 'smooth_ticks_d'

    // set 0
    course_p0_ = glm::mix( course_p0_, course_p1_, alpha );
    auto v = glm::mix( course_u0_, course_u1_, alpha );
    course_u0_ = glm::length( v ) == 0 ? v : glm::normalize( v );
    if ( !map_view ) course_d0_ = smooth( course_d0_, course_d1_, alpha_d );

    // set 1
    forest::Control* control0 = demo.course[ demo.course_i ];
    forest::Control* control1 = demo.course[ demo.course_i + 1 ];
    auto p0 = glm::vec2( control0->aim.pos.x, control0->aim.pos.z ); 
    auto p1 = glm::vec2( control1->aim.pos.x, control1->aim.pos.z ); 
    course_p1_ = 0.5f * (p0 + p1);
    course_u1_ = glm::normalize( p1 - p0 );
    if ( !map_view ) course_d1_ = glm::distance( p0, p1 ); 

    // if starting a new course, set at once
    if ( demo.course_i == 0 )
    {
        course_u0_ = course_u1_;
        course_p0_ = course_p1_;
        if ( !map_view ) course_d0_ = course_d1_;
    }

    course_tick_ = run.tick;
    if ( !map_view ) course_tick_d_ = run.tick;



}

} // namespace demo

} // namespace batb



