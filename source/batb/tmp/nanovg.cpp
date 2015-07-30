//    open-forest: an orientering game.
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
#include "batb/BATB.hpp"
#include "batb/tmp/nanovg.hpp"
#include "batb/tmp/nanovg/demo.h"
#include "batb/tmp/nanovg/perf.h"

namespace batb
{


namespace tmp
{

namespace nanovg
{


static struct DemoData data;
static struct PerfGraph fps;
static bool tmp_empty = true;


void demo_begin(BATB& batb)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    if ( tmp_empty )
    {
	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	if (loadDemoData(batb.gl.nvg_context, &data) == -1)
		return;
    }
    tmp_empty = false;
}


void demo_end(BATB& batb)
{
    if ( !tmp_empty )
    {
        freeDemoData(batb.gl.nvg_context, &data); 
    }
}



void demo_iterate(BATB& batb, bool premult, bool blowup)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

/*
    // set for next iteration...
    if (premult)
            glClearColor(0,0,0,0);
    else
            glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
*/   
    if ( tmp_empty )
    {
        return;
    }

    // begin GL state for nanovg
    gl::begin_nanovg();

    // compute
    static tick_t tick_prev = 0.0;
    tick_t tick = glfwGetTime();
    tick_t dt = tick - tick_prev;
    tick_prev = tick;

    updateGraph(&fps, dt);


    GLFWwindow* win = glfwGetCurrentContext();
    double mx, my;
    glfwGetCursorPos( win, &mx, &my);
    int winWidth, winHeight;
    glfwGetWindowSize( win, &winWidth, &winHeight );
    int fbWidth, fbHeight;
    glfwGetFramebufferSize( win, &fbWidth, &fbHeight );

    // Calculate pixel ration for hi-dpi devices.
    float pxRatio = (float)fbWidth / (float)winWidth;

    nvgBeginFrame(batb.gl.nvg_context, winWidth, winHeight, pxRatio, premult ? NVG_PREMULTIPLIED_ALPHA : NVG_STRAIGHT_ALPHA);

    renderDemo(batb.gl.nvg_context, mx,my, winWidth,winHeight, tick, blowup, &data);
    renderGraph(batb.gl.nvg_context, 5,5, &fps);

    nvgEndFrame(batb.gl.nvg_context);


    // end GL state for nanovg
    gl::end_nanovg();
}





} // namespace nanovg

} // namespace tmp


} // namespace batb




