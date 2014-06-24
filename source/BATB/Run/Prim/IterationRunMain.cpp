#include "BATB/Run.hpp"
#include "BATB/Run/Prim/Scene.hpp"
#include "BATB/GUI.hpp"

// this must only be included in 1 source file, because no inline functions!
//#define NANOVG_GL3_IMPLEMENTATION // this did not work for me :(  FIXME
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"


namespace BATB
{


    // tmp
    static struct DemoData data;
    static struct NVGcontext* vg = 0;
    static struct PerfGraph fps;
    static bool tmp_empty = true;
    void tmp_output_begin();
    void tmp_output_iterate();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IterationRunMain0* IterationRunMain0::create(xml::XMLElement* elem)
{
    log << __PRETTY_FUNCTION__ << std::endl;
    static IterationRunMain1 iter1;
    static IterationRunMain0 iter0( &iter1 );

    // FIXME: create iter0 (cannot be overloaded)

    iter1.create( elem );

    return &iter0;
}


void IterationRunMain0::destroy(IterationRunMain0* iter0)
{
    // destroy 'iter'
    iter0->next()->destroy();
    iter0->destroy();

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void IterationRunMain1::create(xml::XMLElement* elem)
{
    using namespace xml;

    XMLHandle xml( elem );
    // init data for this from xml config
    
}

void IterationRunMain1::destroy()
{

}

void IterationRunMain0::destroy()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void IterationRunMain0::world_begin(RunWorld& run)
{
    log << "IterationRunMain0::iterate" << std::endl;

    // create Scene
    uint wth, hth;
    Env::screenSize( wth, hth );
    run.scene()->size( wth, hth );

    theRun()->keys->keys_clear(); 

    log << std::endl << "\n\npress INSERT to toggle old-BATB..." << std::endl;
    log << "click ESCAPE to exit from Main...\n\n" << std::endl;

    // nanovg demo
    tmp_output_begin();

    // GUI
    theGUI()->keysCallback();
    // add windows...
    // just dynamically allocate without manual delete, memory management is performed by TB
    tb::TBWindow* window = new tb::TBWindow();
    window->SetRect( tb::TBRect(0, 0, 128, 64 ) );
    theGUI()->root()->AddChild( window );
    // application->GetRoot()->AddChild(this);
}


void IterationRunMain1::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    ////////////////////////////////////////////////////////////////////////////////
    //  OUTPUT
    //

    // begin Scene for this frame
    // FIXME: set our GL-state?
    scene_begin( run.scene() );

    // nanovg demo:
    tmp_output_iterate();

    // FIXME: create helpers for this
    // set back GL after nanovg
    // see old.cpp and README.md for nanovg
    glEnable( GL_DEPTH_TEST );
    glBindBuffer(GL_ARRAY_BUFFER, 0); // this seems to be the problem if no other output that nanovg...


    // GUI
    // FIXME: Scene wth/hth
    uint s_wth, s_hth;
    Env::screenSize( s_wth, s_hth );
    theGUI()->output( s_wth, s_hth );

    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    // 

    // GUI
    theGUI()->step();

    theRun()->keys->keys_update( Env::tick() );
    
    float_t wth = run.scene()->shape()->wth;
    float_t hth = run.scene()->shape()->hth;


    KeyPointer* pointer = theForest()->keys->pointer;
    float_t x0, y0, x1, y1;
    tick_t ticks;
    if ( pointer->drag( wth, hth, x0, x1, y0, y1, ticks ) )
    {
        std::printf("drag (%#5.2f, %#5.2f) -> (%#5.2f, %#5.2f), time %#5.2f \n", x0, x1, y0, y1, ticks );
    }
    if ( pointer->drop( wth, hth, x0, x1, y0, y1, ticks ) )
    {
        std::printf("drop (%#5.2f, %#5.2f) -> (%#5.2f, %#5.2f), time %#5.2f \n", x0, x1, y0, y1, ticks );
    }

    float_t x;
    float_t y;
    if ( pointer->right_pressed( wth, hth, x, y ) )
    {
        std::printf("right down: (%#5.2f, %#5.2f) \n", x, y );
    }
    if ( pointer->right_released( wth, hth, x, y ) )
    {
        std::printf("right up:   (%#5.2f, %#5.2f) \n", x, y );
    }

    if ( theRun()->keys->pause->click() )
    {
        log << "IterationRunMain ->" << std::endl;
        stack.push();
        return;
    }

    // run old-BATB if INSERT released
    if ( theRun()->keys->old->released() )
    {
        log << "IterationRunMain -> IterationRunOld" << std::endl;
        stack.push( theRun()->prim->iterationRunOld, this );
        return;
    }


    // if no other stack returns, continue with this
    stack.push( this );

}


void tmp_output_begin()
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

void tmp_output_iterate()
{
    static bool premult = false;
    static bool blowup = false;
 
    // toggle stuff
    if ( theRun()->keys->u->click() )
    {
        premult = !premult;
    }
    if ( theRun()->keys->i->click() )
    {
        blowup = !blowup;
    }
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
    tick_t tick = Env::tick();
    tick_t dt = tick - tick_prev;
    tick_prev = tick;

    updateGraph(&fps, dt);

    double mx, my;
    glfwGetCursorPos(Env::screenWindow(), &mx, &my);
    int winWidth, winHeight;
    glfwGetWindowSize(Env::screenWindow(), &winWidth, &winHeight);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Env::screenWindow(), &fbWidth, &fbHeight);

    // Calculate pixel ration for hi-dpi devices.
    float pxRatio = (float)fbWidth / (float)winWidth;

    nvgBeginFrame(vg, winWidth, winHeight, pxRatio, premult ? NVG_PREMULTIPLIED_ALPHA : NVG_STRAIGHT_ALPHA);

    renderDemo(vg, mx,my, winWidth,winHeight, tick, blowup, &data);
    renderGraph(vg, 5,5, &fps);

    nvgEndFrame(vg);

}


}

