#include "batb/old/old.hpp"

#include <plib/pu.h>
#include "mainWindow.h"
#include "texture.h"
#include "fdManager.h"
#include "server.h"
#include "shader.h"
#include "noise.h"


namespace old
{

static bool do_exit = false;

void begin()
{
    

    // old-BATB didn't like to be initialized more than once :(
    // hence, ignore re-init for specified parts, and continue with previous state
    static bool empty = true;
    
    // clear exit, making possible to start over again
    //do_exit = false;

        // copied from old::main:
        //MainWindow::winPosX = 100;
        //MainWindow::winPosY = 100;
        //MainWindow::winWidth = 640;
        //MainWindow::winHeight = 480;

        // init glut
        //glutInitWindowPosition(MainWindow::winPosX, MainWindow::winPosY);
        //glutInitWindowSize    (MainWindow::winWidth, MainWindow::winHeight);
        //glutInit              (&argc, argv);
        //glutInitDisplayMode   (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        //MainWindow::main_window = glutCreateWindow("Beat About the Bush");
        MainWindow::enableDisplayFunc();

        // implemented as GLFW:

        glfwSetKeyCallback( env::screen_window(), MainWindow::glfwKey  );                  //glutKeyboardFunc      (MainWindow::keyDownFn); 
                                                                                          //glutSpecialFunc       (MainWindow::specialDownFn);
                                                                                          ////glutKeyboardUpFunc    (MainWindow::keyUpFn);
                                                                                          ////glutSpecialUpFunc     (MainWindow::specialUpFn);
        glfwSetCursorPosCallback( env::screen_window(), MainWindow::glfwCursorPos );       //glutMouseFunc         (MainWindow::mousefn); 
        glfwSetMouseButtonCallback( env::screen_window(), MainWindow::glfwMouseButton);    //glutMotionFunc        (MainWindow::motionfn);
                                                                                          
        glfwSetWindowSizeCallback( env::screen_window(), MainWindow::glfwWindowSize );     //glutReshapeFunc       (MainWindow::reshapefn);
        glfwSetWindowFocusCallback( env::screen_window(), MainWindow::glfwWindowFocus );   //glutVisibilityFunc    (MainWindow::visibility);
        
        
        
        
    if ( empty )
    {

        // init plib, with no GLUT-binding!
        puInitOLD();

        puSetDefaultStyle        ( PUSTYLE_SMALL_SHADED );
        puSetDefaultColourScheme ( 0.3f, 0.4f, 0.6f, 1.0f);

        // Initialize the "OpenGL Extension Wrangler" library
        //glewInit();
    }

        mainWindow.initLights();

    if ( empty )
    {
        mainWindow.init();
        printVersions();

        // Make sure that OpenGL 2.0 is supported by the driver
        int gl_major, gl_minor;
        getGlVersion(&gl_major, &gl_minor);
        printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);

        if (gl_major < 2)
        {
            printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
            printf("Support for OpenGL 2.0 is required for this demo...exiting\n");
            //exit(1);
            old::exit(1);
            return;
        }

        // init shaders
        GLchar *dayVSSource, *dayFSSource;
        readShaderSource( old::file("shader/day").c_str(), &dayVSSource, &dayFSSource);
        dayShader = installShaders(dayVSSource, dayFSSource);
        float forestGreen[] = {34.0/255, 139.0/255, 34.0/255};
        //float auburn[] = {113.0/255, 47.0/255, 38.0/255};
        float grey[] = {0.5, 0.5, 0.5};
        //float sepia[] = {112.0/255, 66.0/255, 20.0/255};

        setUniform3f(dayShader, "GrassColor", forestGreen[0], forestGreen[1], forestGreen[2]);
        setUniform3f(dayShader, "RockColor", grey[0], grey[1], grey[2]);
        //setUniform3f(dayShader, "DirtColor", sepia[0], sepia[1], sepia[2]);
        //setUniform4f(dayShader, "LightPos", 0.0, 0.0, 100.0, 1.0);
        setUniform1f(dayShader, "Scale", 1.0);
        setUniform1f(dayShader, "TerrainHigh", 0.1);
        setUniform1f(dayShader, "TerrainLow", 0.1);
        setUniform1i(dayShader, "Trees", 0); // sampler
        setUniform1i(dayShader, "AltGrad", 1); // sampler
        setUniform1i(dayShader, "Noise", 2); // sampler

        GLchar *nightVSSource, *nightFSSource;
        readShaderSource( old::file("shader/night3").c_str(), &nightVSSource, &nightFSSource);
        nightShader = installShaders(nightVSSource, nightFSSource);
        setUniform3f(nightShader, "BrickColor", 1.0, 0.3, 0.2);
        setUniform3f(nightShader, "MortarColor", 0.85, 0.86, 0.84);
        setUniform2f(nightShader, "BrickSize", 0.30, 0.15);
        setUniform2f(nightShader, "BrickPct", 0.90, 0.85);
        setUniform1i(nightShader, "numEnabledLights", 2);

        setUniform3f(nightShader, "GrassColor", forestGreen[0], forestGreen[1], forestGreen[2]);
        setUniform3f(nightShader, "RockColor", grey[0], grey[1], grey[2]);
        setUniform1f(nightShader, "Scale", 1.0);
        setUniform1f(nightShader, "TerrainHigh", 0.1);
        setUniform1f(nightShader, "TerrainLow", 0.1);
        setUniform1i(nightShader, "Trees", 0); // sampler
        setUniform1i(nightShader, "AltGrad", 1); // sampler
        setUniform1i(nightShader, "Noise", 2); // sampler

        GLchar *nightTreeVS, *nightTreeFS;
        readShaderSource( old::file("shader/nightTree").c_str(), &nightTreeVS, &nightTreeFS);
        nightTrees = installShaders(nightTreeVS, nightTreeFS);
        setUniform1i(nightTrees, "Trees", 0); // sampler

        // BUGFIX:
        free( dayVSSource );
        free( dayFSSource );
        free( nightVSSource );
        free( nightFSSource );
        free( nightTreeVS );
        free( nightTreeFS );
    }

        // enter main loop
        //if (dayShader && nightShader)
        //    glutMainLoop();

        //return 0;

    empty = false;

}

void end()
{
    // FIXME: old-BATB specific shutdown

    // remove key callbacks
    glfwSetKeyCallback( env::screen_window(), 0  );                  //glutKeyboardFunc      (MainWindow::keyDownFn); 
    glfwSetCursorPosCallback( env::screen_window(), 0 );       //glutMouseFunc         (MainWindow::mousefn); 
    glfwSetMouseButtonCallback( env::screen_window(), 0 );    //glutMotionFunc        (MainWindow::motionfn);
    glfwSetWindowSizeCallback( env::screen_window(), 0 );     //glutReshapeFunc       (MainWindow::reshapefn);
    glfwSetWindowFocusCallback( env::screen_window(), 0 );   //glutVisibilityFunc    (MainWindow::visibility);




}

void iterate()
{
    // see: nanovg_gl.h: static void glnvg__renderFlush(void* uptr, int alphaBlend)
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_STENCIL_TEST );
    glDisable( GL_SCISSOR_TEST );
    glDisable( GL_CULL_FACE );
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);

	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
        
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

    // force reshape
    env::uint wth, hth;
    env::screen_size( wth, hth );
    MainWindow::reshapefn( wth, hth );

    // "glut display func"
    if ( DisplayFunc disp = get_display_func() )
    {
        // set the clear color old-BATB has set in MainWindow::displayXXX:
        if ( disp == MainWindow::mapDF )
        {
            glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        }
        else if ( disp == MainWindow::terrainDF )
        {
            Database& db = Database::instance();
            Event* event = db.getSelectedEvent();
            bool night = event->night();
            if ( night )
            {
                glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
            }
            else
            {
                glClearColor(0.0, 0.746, 1.0, 1.0);
            }
        }
        else
        {
            // from 'initLights'
            GLfloat day_clear_r   = 0.541; GLfloat day_clear_g   = 0.6196; GLfloat day_clear_b = 1.0;
            glClearColor(day_clear_r, day_clear_g, day_clear_b, 1.0);
        }

        // display a frame
        disp();
    }


    // reset the OpenGL state to what new-BATB assumes
    // (see MainWindow::displayMap/MainWindow::displayTerrain)
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glShadeModel(GL_SMOOTH);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}


void exit(int err)
{
    std::cout << THIS_FUNCTION << err << std::endl;
    //old::end(); we cant end here, it gives segfault (glfwPollEvents)
    do_exit = true;
}

bool is_exit()
{
    return do_exit;
}


std::string file(const char* path)
{
    // executable is built into "build/" folder, point to data dir
    static const std::string prefix = "data/old/";

    return prefix + path;
}

int getElapsedTime()
{
    // assuming not too long elaps from env::init to old start.
    env::tick_t tick = env::tick();
    return (int)( tick * 1000.0 );
}



// we need these for display functions, since we are not using GLUT:
// also, the old-BATB display functions drive the whole game, i.e. iterates it.

static DisplayFunc display_func_ = 0;

void set_display_func(DisplayFunc f)
{
    display_func_ = f;

}

DisplayFunc get_display_func()
{
    return display_func_;
}


} // namespace old
