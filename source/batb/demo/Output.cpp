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
#include <iomanip>
#include "batb/demo/Output.hpp"
#include "batb/demo.hpp"
#include "batb/demo/World.hpp"
#include "batb.hpp"
#include "batb/CourseDrawer.hpp"
#include "batb/value/run.hpp"


namespace batb
{

namespace demo
{

  
constexpr tick_t smooth_ticks = 1.5;

inline void nvg_point(NVGcontext* vg, float_t x, float_t y, float r = 14.0)
{
    nvgBeginPath(vg);
    nvgArc( vg, x, y, r, 0, twopi, NVG_CCW );
    nvgFill( vg );
    //nvgRoundedRect(vg, x - r, y - r, 2 * r, 2 * r, 0.5 * r );
    //nvgStroke( vg );
}

Output::Output(BATB& b) : batb( b )
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
        const auto& pos = demo.runner->move.aim[3];
        auto& course = demo.course;

        // update view according to World
        update( demo );


        ////////////////////////////////////////////////////////////////////////////////
        // begin nanovg drawing
        auto* nvg = batb.gl.nanovg_begin( run.scene );

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

        float_t alpha = course_tick_ + smooth_ticks <= demo.tick ? 1.0 : (demo.tick - course_tick_) / smooth_ticks;
        float_t alpha_d = course_tick_d_ + smooth_ticks <= demo.tick ? 1.0 : (demo.tick - course_tick_d_) / smooth_ticks;

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
                auto& pos = control->aim[3];

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
        batb.gl.nanovg_end();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // TODO: other output
    //  * sound 
    //  * network
    //  * 

}


void Output::update(World& demo)
{
    if ( !updated_ )
    {
        aim( demo );
        updated_ = true;
    }

    forest::Control* control = demo.course[ demo.course_i ];
    auto p0 = course_p0_;
    auto p1 = glm::vec2( control->aim[3].x, control->aim[3].z );
  
    // set new aim, if changed
    if ( p0 != p1 )
    {

        aim( demo );
        course_i_ = demo.course_i;
    }

}


void Output::aim(World& demo)
{
    float_t alpha = keep_inside( 0.0, 1.0, (demo.tick - course_tick_) / smooth_ticks );
    float_t alpha_d = keep_inside( 0.0, 1.0, (demo.tick - course_tick_d_) / smooth_ticks );

    // set 0
    course_p0_ = glm::mix( course_p0_, course_p1_, alpha );
    auto v = glm::mix( course_u0_, course_u1_, alpha );
    course_u0_ = glm::length( v ) == 0 ? v : glm::normalize( v );
    course_d0_ = smooth( course_d0_, course_d1_, alpha_d );

    // set 1
    forest::Control* control0 = demo.course[ demo.course_i ];
    forest::Control* control1 = demo.course[ demo.course_i + 1 ];
    auto p0 = glm::vec2( control0->aim[3].x, control0->aim[3].z ); 
    auto p1 = glm::vec2( control1->aim[3].x, control1->aim[3].z ); 
    course_p1_ = 0.5f * (p0 + p1);
    course_u1_ = glm::normalize( p1 - p0 );
    course_d1_ = glm::distance( p0, p1 );

    // if starting a new course, set at once
    if ( demo.course_i == 0 )
    {
        course_u0_ = course_u1_;
        course_p0_ = course_p1_;
        course_d0_ = course_d1_;
    }

    course_tick_ = demo.tick;
    course_tick_d_ = demo.tick;



}

} // namespace demo

} // namespace batb



