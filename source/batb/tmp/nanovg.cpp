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
static struct NVGcontext* vg = 0;
static struct PerfGraph fps;
static bool tmp_empty = true;


void demo_begin()
{
    if ( tmp_empty )
    {
#ifdef NANOVG_GL2_IMPLEMENTATION
        vg = nvgCreateGL2(512, 512, 0);
#endif
#ifdef NANOVG_GL3_IMPLEMENTATION
	vg = nvgCreateGL3(512, 512, 0);
#endif
	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return;
	}

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	if (loadDemoData(vg, &data) == -1)
		return;
    }
    tmp_empty = false;
}

void demo_iterate(bool premult, bool blowup)
{
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

    // compute
    static tick_t tick_prev = 0.0;
    tick_t tick = env::tick();
    tick_t dt = tick - tick_prev;
    tick_prev = tick;

    updateGraph(&fps, dt);

    double mx, my;
    glfwGetCursorPos( env::screen_window(), &mx, &my);
    int winWidth, winHeight;
    glfwGetWindowSize( env::screen_window(), &winWidth, &winHeight );
    int fbWidth, fbHeight;
    glfwGetFramebufferSize( env::screen_window(), &fbWidth, &fbHeight );

    // Calculate pixel ration for hi-dpi devices.
    float pxRatio = (float)fbWidth / (float)winWidth;

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio, premult ? NVG_PREMULTIPLIED_ALPHA : NVG_STRAIGHT_ALPHA);

    renderDemo(vg, mx,my, winWidth,winHeight, tick, blowup, &data);
    renderGraph(vg, 5,5, &fps);

    nvgEndFrame(vg);


    // set back GL after nanovg
    gl::reset_nanovg();

}





} // namespace nanovg

} // namespace tmp


} // namespace batb




