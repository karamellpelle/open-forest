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

    // output
        gl::begin_nanovg();

        int winWidth, winHeight;
        glfwGetWindowSize( batb.env.window, &winWidth, &winHeight );
        int fbWidth, fbHeight;
        glfwGetFramebufferSize( batb.env.window, &fbWidth, &fbHeight );
        float pxRatio = (float)fbWidth / (float)winWidth; // Calculate pixel ration for hi-dpi devices.

        auto ctx = batb.gl.nvg_context;
        auto wth = demo.run.scene.wth;
        auto hth = demo.run.scene.hth;
        nvgBeginFrame( ctx, wth, hth, pxRatio);
        nvgSave( ctx );

        nvgStrokeColor(ctx, nvgRGBA(255,0,0,160));
        nvgFillColor(ctx, nvgRGBA(255,0,0,160));
        nvg_point( ctx, 50, 50 );
        nvg_point( ctx, 150, 150 );
        nvg_point( ctx, 250, 50 );

        // TODO: scale!
        // set origo in the middle
        nvgTranslate( ctx, wth / 2, hth / 2 );

        constexpr float_t dim = 600.0;
        float_t sx = wth / (dim * run.scene.shape.wth);
        float_t sy = hth / (dim * run.scene.shape.hth);
        nvgScale( ctx, sx, sy );
        // (move to origo, scale (-0.0, -0.0)

        auto moveTo = [&](const TracePoint& p) { nvgMoveTo( ctx, p.x, p.z ); };
        auto lineTo = [&](const TracePoint& p) { nvgLineTo( ctx, p.x, p.z ); };


        nvgStrokeWidth( ctx, 4 );
        nvgLineCap(ctx, NVG_ROUND );
        nvgStrokeColor(ctx, nvgRGBA(0,192,255,128));

        const auto& points = trace.points;
        if ( !points.empty() )
        {
        nvgBeginPath(ctx);
            moveTo( points[0] );
            for (uint i = 1; i != points.size(); ++i)
            {
                lineTo( points[i] );
            }
            lineTo( trace.point0 );
        nvgStroke(ctx);
            
        }

        // draw current position
        nvgStrokeColor(ctx, nvgRGBA(255,0,0,255));
        nvgFillColor(ctx, nvgRGBA(255,0,0,255));
        nvg_point( ctx, pos.x, pos.z ); 
        
    std::cout << "\r pos: "
              << "( "
              << std::setprecision( 1 )
              << std::fixed
              << pos.x << ", "
              << pos.z << ") "
              << std::endl;

        nvgRestore( ctx );
        nvgEndFrame( ctx );

        gl::end_nanovg();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // TODO: other output
    //  * sound 
    //  * network
    //  * 
}


} // namespace demo

} // namespace batb



