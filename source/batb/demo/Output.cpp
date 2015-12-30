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


namespace batb
{

namespace demo
{

  
inline void nvg_point(NVGcontext* vg, float_t x, float_t y, float r = 12.0)
{
    nvgBeginPath(vg);
    nvgArc( vg, x, y, r, 0, twopi, NVG_CCW );
    nvgFill( vg );
    //nvgRoundedRect(vg, x - r, y - r, 2 * r, 2 * r, 0.5 * r );
    //nvgStroke( vg );
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
        
        // scaling
        uint min = std::min( wth, hth );
        uint pad = 100 <= min ? 100 : min;
        float_t scale = (float_t)(min - pad) / (2.0 * course.dimension()); // / (float_t)( min - pad );
        nvgScale( nvg, scale, scale );

        // center at start control
        auto start = course[0]->aim[3];
        nvgTranslate( nvg, -start.x, -start.z );

        ////////////////////////////////////////////////////////////////////////////////
        // draw trace

        auto moveTo = [&](const TracePoint& p) { nvgMoveTo( nvg, p.x, p.z ); };
        auto lineTo = [&](const TracePoint& p) { nvgLineTo( nvg, p.x, p.z ); };

        nvgStrokeWidth( nvg, 16 );
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
            drawer.size( 64 );

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


} // namespace demo

} // namespace batb



