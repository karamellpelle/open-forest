//    open-forest: an orientering game.
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
#include "BATB/Demo/libs/nanovg.hpp"
#include "BATB/Demo/libs/nanovg/demo.h"
#include "BATB/Demo/libs/nanovg/perf.h"
#include "BATB/GL.hpp"
#include "BATB/Screen.hpp"

namespace batb
{


namespace demo
{

namespace nanovg
{



static GLFWwindow* window;
static DemoData data;
static NVGcontext* vg = NULL;
static PerfGraph fps;
static double prevt = 0;

static bool tmp_empty = true;
static bool on_ = false;


void demo_begin(BATB* batb)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    if ( tmp_empty )
    {
	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

	if (loadDemoData(batb->gl->nvg_context, &data) == -1)
		return;
    }
    tmp_empty = false;
}


void demo_end(BATB* batb)
{
    if ( !tmp_empty )
    {
        freeDemoData(batb->gl->nvg_context, &data); 
    }
}


void demo_on(bool b)
{
    on_ = b;
}

void demo_toggle()
{
    on_ = !on_;
}

void demo_iterate(BATB* batb, bool premult, bool blowup)
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    
/*
    // set for next iteration...
    if (premult)
            glClearColor(0,0,0,0);
    else
            glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
*/   
    if ( tmp_empty ) return;
    if ( !on_ )  return;

    // begin GL state for nanovg
    batb->gl->nanovgBegin();
    window = batb->screen->glfw_window;
    vg = batb->gl->nvg_context;

		double mx, my, t, dt;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;

		t = glfwGetTime();
		dt = t - prevt;
		prevt = t;
		updateGraph(&fps, dt);

		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		//glViewport(0, 0, fbWidth, fbHeight);
		//if (premult)
		//	glClearColor(0,0,0,0);
		//else
		//	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

		renderDemo(vg, mx,my, winWidth,winHeight, t, blowup, &data);
		renderGraph(vg, 5,5, &fps);

		nvgEndFrame(vg);


    // end GL state for nanovg
    batb->gl->nanovgEnd();
}





} // namespace nanovg

} // namespace demo


} // namespace batb




