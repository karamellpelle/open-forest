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

#if 0
#include <iostream>
#include <sstream>
#include <GL/glew.h>    // must be done before GLFW!!
#include <GLFW/glfw3.h> // FIXME: build option?

extern "C"
{

static void glfw_error_callback(int error, const char* str)
{
    std::cerr << "GLFW error (code " << error << "): " << str << std::endl;
}

}


void f()
{

    ////////////////////////////////////////////////////////////////////////////////
    // GLFW
    glfwSetErrorCallback( glfw_error_callback );

    if ( !glfwInit() )
    {
        throw std::runtime_error( "env::begin: could not init GLFW" );
    }

    // set all hints to defaults
    glfwDefaultWindowHints();

    // set hints to window
    // http://www.glfw.org/docs/latest/window.html#window_hints
    //glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
    //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_CORE_PROFILE /* GLFW_COMPAT_PROFILE */ ); // OpenGL 3.2+
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );     // debug symbols (?) 
    glfwWindowHint( GLFW_DECORATED, GL_FALSE );               // we do not need a close button.


    // size
    uint wth = 640;
    uint hth = 480;

    // fullscreen
    bool fullscreen = false;

    // multisamples
    uint samples = 0;
    //if ( YAML::Node node = screen[ "multisamples" ] )
    //{
    //    samples = node.as<uint>( samples );
    //    glfwWindowHint( GLFW_SAMPLES, samples ); 
    //}

    // title
    std::string title = "test-glew";

    GLFWmonitor* monitor = nullptr;
    
    // NOTE: error in implementation of glfw, according to valgrind:
    auto* window = glfwCreateWindow( wth, hth, title.c_str(), monitor, 0 );

    // set GL context as 'theWindow_'
    glfwMakeContextCurrent( window );

    if ( !window )
    {
        throw std::runtime_error( "env::begin: could not create window" );
    }

std::cout << "before glew:\n";
std::cout << "glEnable: " << &glEnable << std::endl;
std::cout << "glClearColor: " << &glClearColor << std::endl;
std::cout << "glBlendEquationSeparate: " << &glBlendEquationSeparate << std::endl;

    // we now have a context, init GLEW
    GLenum err = glewInit();
    if ( err != GLEW_OK )
    {
        std::ostringstream os;
        os << "env::begin: could not init GLEW (" << glewGetErrorString( err ) << ")";
        throw std::runtime_error( os.str() );
    }

std::cout << std::endl;
std::cout << "****************************************************************" << std::endl;
std::cout << "after glew:\n";
std::cout << "glEnable: " << &glEnable << std::endl;
std::cout << "glClearColor: " << &glClearColor << std::endl;
std::cout << "glBlendEquationSeparate: " << &glBlendEquationSeparate << std::endl;

//  set up our GL-invariants:
glEnable( GL_MULTISAMPLE );
glClearColor( 0, 0, 0, 0 );
glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );

while ( true )
{
    // populate events
    glfwPollEvents();
    glfwSwapBuffers( window );

}
//while ( true );

}

int main(int argc, char** argv)
{
    f();
}
#endif

#include "include.hpp"
#include "env.hpp"
#include "game.hpp"
#include "file.hpp"
#include "batb.hpp"
#include "batb/run/workers.hpp" 
#include "batb/demo/other.hpp"



//void commandline_env(int argc, char** argv, YAML::Node& yaml)
//{
//
//}
//void commandline_batb(int argc, char** argv, YAML::Node& yaml)
//{
//
//}


int main(int argc, char** argv)
{
    int ret = 0;

    // TODO: use YAML::LoadFile, modify node from cmdline, pass to module.
    //       if so, configurations from command line options will be saved
    //       to file.

    env::Env env;
    env.config( file::dynamic_data( "env/Env.yaml" ) );

    // our BATB object
    batb::BATB batb( env );
    batb.config( file::dynamic_data( "batb/BATB.yaml" ) );


    try
    {
            
        // create our environment
        env::begin( env );

        // initialize the core parts of BATB.
        // (the non-core part is created by 'iterationRunBegin')
        batb::begin( batb );

        batb::demo::other_begin( batb ); // FIXME: remove

        using namespace batb;
        
        // application world
        run::World run;
        run.player = batb::run::local_player();
        

        auto* loadBATB = new run::IterationRunWork( batb, run::LoadWorker<BATB>( batb ) );
        auto* unloadBATB = new run::IterationRunWork( batb, run::UnloadWorker<BATB>( batb ) );
        run::IterationStack stack =
        {
              game::begin_iteration( loadBATB ),                      // create the non-core part of BATB
              game::begin_iteration( batb.run.iterationRunMain ),     // main
              game::begin_iteration( unloadBATB )                     // destroy game data at end
        };

  
  
        // "main loop"
        while ( !stack.empty() )
        {
            // begin frame for iteration
            env.frameBegin();

            // make 1 iteration of 'run'
            iterate( stack, run );

            // end frame for iteration
            env.frameEnd();
        }

        batb::demo::other_end( batb ); // FIXME: remove
    }
    catch (std::exception& e)
    {
        // some serious error occured above, lets handle it
        std::cerr << "main: " << e.what() << std::endl;
        ret = 1;
    }


    batb::end( batb );

    env::end( env );

    return ret;
}
