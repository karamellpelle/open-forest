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

  
inline void nvg_point(NVGcontext* vg, float_t x, float_t y, float r = 4.0)
{
    //context.beginPath();
    //context.arc(centerX, centerY, radius, 0, 2 * Math.PI, false);
    //context.fillStyle = 'green';
    //context.fill();
    //context.lineWidth = 5;
    //context.strokeStyle = '#003300';
    //context.stroke();
	nvgBeginPath(vg);
        nvgArc( vg, x, y, r, 0, twopi, NVG_CCW );
        nvgFill( vg );
        //nvgStroke( vg );
	//nvgRoundedRect(vg, x - r, y - r, 2 * r, 2 * r, 0.5 * r );
	//nvgStroke
}

void Output::operator()(World& demo)
{
    using namespace forest;

    if ( demo.runner )
    {
        const auto& trace = demo.runner->trace;
        auto& run = demo.run;
        const auto& pos = demo.runner->move.aim[3];


        auto* nvg = batb.gl.nanovg_begin( run.scene );
        nvgSave( nvg );

        CourseDrawer course( batb );
        course.numbers( true );
        course.size( 18 );

        course.begin();
        course.start( glm::vec2( 40, 40 ) );  
        course.normal( glm::vec2( 120, 95 ) );
        course.normal( glm::vec2( 200, 140 ) );
        course.normal( glm::vec2( 210, 200 ) );
        course.normal( glm::vec2( 380, 240 ) );
        course.normal( glm::vec2( 470, 80 ) );
        course.normal( glm::vec2( 520, 76 ) );
        course.normal( glm::vec2( 580, 300 ) );
        course.finish( glm::vec2( 670, 320 ) );
        course.end();

        //////////////////////////////////////////////////////////////////////////////////

        // set origo in the middle and normalize screen 
        auto wth = demo.run.scene.wth;
        auto hth = demo.run.scene.hth;
        nvgTranslate( nvg, wth / 2, hth / 2 );
        batb.gl.nanovg_normalize( run.scene );

        constexpr float_t dim = 1.0 / 600.0; // terrain dimension
        nvgScale( nvg, dim, dim );

        auto moveTo = [&](const TracePoint& p) { nvgMoveTo( nvg, p.x, p.z ); };
        auto lineTo = [&](const TracePoint& p) { nvgLineTo( nvg, p.x, p.z ); };

        //////////////////////////////////////////////////////////////////////////////////

        nvgStrokeWidth( nvg, 4 );
        nvgLineCap(nvg, NVG_ROUND );
        nvgStrokeColor(nvg, nvgRGBA(0,192,255,128));

        const auto& points = trace.points;
        if ( !points.empty() )
        {
        nvgBeginPath(nvg);
            constexpr uint tails = 32; 
            uint b = tails <= points.size() ? points.size() - tails : 0;
            uint e = points.size();
            moveTo( points[b] );
            for (uint i = b + 1; i != points.size(); ++i)
            {
                lineTo( points[i] );
            }
            lineTo( trace.point0 );
        nvgStroke(nvg);
            
        }

        // draw current position
        nvgStrokeColor(nvg, nvgRGBA(255,0,0,255));
        nvgFillColor(nvg, nvgRGBA(255,0,0,255));
        nvg_point( nvg, pos.x, pos.z ); 
        
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



