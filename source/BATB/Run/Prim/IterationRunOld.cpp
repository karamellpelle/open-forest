#include "BATB/Run.hpp"
#include "BATB/Run/Prim/Scene.hpp"


#include <plib/pu.h>
#include "mainWindow.h"



namespace BATB
{


static bool exit_ = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IterationRunOld0* IterationRunOld0::create(xml::XMLElement* elem)
{
    log << __PRETTY_FUNCTION__ << std::endl;
    static IterationRunOld1 iter1;
    static IterationRunOld0 iter0( &iter1 );

    // FIXME: create iter0 (cannot be overloaded)

    iter1.create( elem );

    return &iter0;
}


void IterationRunOld0::destroy(IterationRunOld0* iter0)
{
    // destroy 'iter'
    iter0->next()->destroy();
    iter0->destroy();

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void IterationRunOld1::create(xml::XMLElement* elem)
{
    using namespace xml;

    XMLHandle xml( elem );
    // init data for this from xml config
    
}

void IterationRunOld1::destroy()
{

}

void IterationRunOld0::destroy()
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void IterationRunOld0::world_begin(RunWorld& run)
{
    log << "IterationRunOld0::iterate" << std::endl;

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
    
    
    
    

    // init plib
    puInit();

    puSetDefaultStyle        ( PUSTYLE_SMALL_SHADED );
    puSetDefaultColourScheme ( 0.3f, 0.4f, 0.6f, 1.0f);

    // Initialize the "OpenGL Extension Wrangler" library
    //glewInit();

    mainWindow.initLights();
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
        exit_ = true;
    }

    // init shaders
    GLchar *dayVSSource, *dayFSSource;
    readShaderSource("shader/day", &dayVSSource, &dayFSSource);
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
    readShaderSource("shader/night3", &nightVSSource, &nightFSSource);
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
    readShaderSource("shader/nightTree", &nightTreeVS, &nightTreeFS);
    nightTrees = installShaders(nightTreeVS, nightTreeFS);
    setUniform1i(nightTrees, "Trees", 0); // sampler

    // FIXME: set display funcs...

    // enter main loop
    //if (dayShader && nightShader)
    //    glutMainLoop();

    //return 0;
}


void IterationRunOld1::iterate(IterationStackRunWorld& stack, RunWorld& run)
{
    // handle old-errors
    if ( exit_ ) return;

    // begin Scene for this frame
    scene_begin( theScene() );
    
    theRun()->keys->keys_update( Env::tick() );


    // FIXME: glutIterate


    if ( theRun()->keys->pause->click_double() )
    {
        stack.push();
    }
    else
    {
        stack.push( this );
    }

}


}

