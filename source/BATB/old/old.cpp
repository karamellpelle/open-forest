#include "old.hpp"

#include <plib/pu.h>
#include "mainWindow.h"
#include "texture.h"
#include "fdManager.h"
#include "server.h"
#include "shader.h"
#include "noise.h"

void old_begin()
{
    
    // old-BATB did not like our GL-state...
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_STENCIL_TEST );
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

    // old-BATB didn't like to be initialized more than once :(
    // hence, ignore re-init for specified parts, and continue with previous state
    static bool empty = true;
    
    // also, if there where a previous error ("system exit"), just ignore all
    if ( BATB::is_exit() )
    {
        return;
    }
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

        glfwSetKeyCallback( Env::screenWindow(), MainWindow::glfwKey  );                  //glutKeyboardFunc      (MainWindow::keyDownFn); 
                                                                                          //glutSpecialFunc       (MainWindow::specialDownFn);
                                                                                          ////glutKeyboardUpFunc    (MainWindow::keyUpFn);
                                                                                          ////glutSpecialUpFunc     (MainWindow::specialUpFn);
        glfwSetCursorPosCallback( Env::screenWindow(), MainWindow::glfwCursorPos );       //glutMouseFunc         (MainWindow::mousefn); 
        glfwSetMouseButtonCallback( Env::screenWindow(), MainWindow::glfwMouseButton);    //glutMotionFunc        (MainWindow::motionfn);
                                                                                          
        glfwSetWindowSizeCallback( Env::screenWindow(), MainWindow::glfwWindowSize );     //glutReshapeFunc       (MainWindow::reshapefn);
        glfwSetWindowFocusCallback( Env::screenWindow(), MainWindow::glfwWindowFocus );   //glutVisibilityFunc    (MainWindow::visibility);
        
        
        
        
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
            BATB::exit(1);
            return;
        }

        // init shaders
        GLchar *dayVSSource, *dayFSSource;
        readShaderSource( old_file("shader/day").c_str(), &dayVSSource, &dayFSSource);
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
        readShaderSource( old_file("shader/night3").c_str(), &nightVSSource, &nightFSSource);
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
        readShaderSource( old_file("shader/nightTree").c_str(), &nightTreeVS, &nightTreeFS);
        nightTrees = installShaders(nightTreeVS, nightTreeFS);
        setUniform1i(nightTrees, "Trees", 0); // sampler
    }
        // force reshape
        Env::uint wth, hth;
        Env::screenSize( wth, hth );
        MainWindow::reshapefn( wth, hth );

        // enter main loop
        //if (dayShader && nightShader)
        //    glutMainLoop();

        //return 0;

    empty = false;

}

void old_end()
{
    // remove key callbacks
    glfwSetKeyCallback( Env::screenWindow(), 0  );                  //glutKeyboardFunc      (MainWindow::keyDownFn); 
    glfwSetCursorPosCallback( Env::screenWindow(), 0 );       //glutMouseFunc         (MainWindow::mousefn); 
    glfwSetMouseButtonCallback( Env::screenWindow(), 0 );    //glutMotionFunc        (MainWindow::motionfn);
    glfwSetWindowSizeCallback( Env::screenWindow(), 0 );     //glutReshapeFunc       (MainWindow::reshapefn);
    glfwSetWindowFocusCallback( Env::screenWindow(), 0 );   //glutVisibilityFunc    (MainWindow::visibility);

    // reset clear color
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    // reset the OpenGL state BATB assumes
    BATB::set_gl_state();

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



namespace BATB
{
    bool old_exit = false;
}
