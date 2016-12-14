//    open-forest: an orienteering game.
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

#include <iostream>
#include <sstream>
#include "env/Env.hpp"

extern "C"
{
static void glfw_error_callback(int error, const char* str)
{
    std::cerr << "GLFW error (code " << error << "): " << str << std::endl;
}
}


namespace env
{

////////////////////////////////////////////////////////////////////////////////
// frame
//

void Env::frameBegin()
{
    tick_ = tickNow();

    // compute frames per second
    constexpr tick_t update_delta = 0.4; // update interval in seconds
    double delta = tick_ - frame_fps_tick_;
    if ( update_delta <= delta )
    {
        frame_fps_ = (frame_count_ - frame_fps_count_) / delta;

        frame_fps_tick_ = tick_; 
        frame_fps_count_ = frame_count_;
    }

    // populate events
    glfwPollEvents();
}

void Env::frameEnd()
{
    glfwSwapBuffers( window );

    ++frame_count_;

}

////////////////////////////////////////////////////////////////////////////////
// screen
//
// TODO: helper FBO (fbo1), change size on window resize

void Env::screenSize(uint& wth, uint& hth)
{
    int w, h;
    glfwGetFramebufferSize( window, &w, &h );
    wth = w;
    hth = h;
}

void Env::screenShape(float_t& wth, float_t& hth)
{
    uint w, h;
    screenSize( w, h );

    float_t scale = 1.0 / std::max( w, h );
    wth = (float_t)( w ) * scale;
    hth = (float_t)( h ) * scale;
}

GLuint Env::screenFBO(GLuint fbo)
{
    GLuint ret = fbo_;

    // TODO: bind 'fbo'
    fbo_ = fbo;

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
// 

void begin(Env& env)
{

    if ( env.init_empty() )
    {
        ////////////////////////////////////////////////////////////////////////////////
        // GLFW
        glfwSetErrorCallback( glfw_error_callback );

        if ( !glfwInit() )
        {
            throw std::runtime_error( "env::begin: could not init GLFW" );
        }

        ////////////////////////////////////////////////////////////////////////////////
        // screen
        YAML::Node screen = env.yaml[ "screen" ];
        // TODO: print GL info if verbose setting in yaml

        GLFWmonitor* monitor = nullptr;

        // set all hints to defaults
        glfwDefaultWindowHints();

        // set hints to window
        // http://www.glfw.org/docs/latest/window.html#window_hints
        //glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
        //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_CORE_PROFILE /* GLFW_COMPAT_PROFILE */ ); // OpenGL 3.2+
        //glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        //glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );     // debug symbols (?) 
        // TODO: parse GLFW hints from yaml

        // decorate window?
        bool decorate = screen["decorate"].as<bool>( false );
        glfwWindowHint( GLFW_DECORATED, decorate );               

        // size
        uint wth = 640;
        uint hth = 480;
        if ( YAML::Node node = screen[ "size" ] )
        {
            wth = node[ "wth" ].as<uint>( wth );
            hth = node[ "hth" ].as<uint>( hth );
        }

        // fullscreen
        bool fullscreen = false;
        if ( YAML::Node node = screen[ "fullscreen" ] )
        {
            monitor = node.as<bool>( fullscreen ) ? glfwGetPrimaryMonitor() : 0;
        }

        // multisamples
        uint samples = 0;
        if ( YAML::Node node = screen[ "multisamples" ] )
        {
            samples = node.as<uint>( samples );
            glfwWindowHint( GLFW_SAMPLES, samples ); 
        }

        // title
        std::string title = "";
        if ( YAML::Node node = screen[ "title" ] )
        {
            title = node.as<std::string>( title );
        }

        
        // NOTE: error in implementation of glfw, according to valgrind:
        env.window = glfwCreateWindow( wth, hth, title.c_str(), monitor, 0 );

        // set GL context as 'theWindow_'
        glfwMakeContextCurrent( env.window );

        if ( !env.window )
        {
            throw std::runtime_error( "env::begin: could not create window" );
        }

        // we now have a context, init GLEW
        GLenum err = glewInit();
        if ( err != GLEW_OK )
        {
            std::ostringstream os;
            os << "env::begin: could not init GLEW (" << glewGetErrorString( err ) << ")";
            throw std::runtime_error( os.str() );
        }

    }

    env.init( true );
}

void end(Env& env)
{
    if ( env.init_nonempty() )
    {
        env.save();

        ////////////////////////////////////////////////////////////////////////////////
        // screen
        glfwDestroyWindow( env.window ); 
        env.window = nullptr;
        env.fbo_ = 0;
        env.fbo0_ = 0;
        env.fbo1_ = 0;

        ////////////////////////////////////////////////////////////////////////////////
        // frame
        env.frame_count_ = 0;
        env.frame_fps_ = 0.0;

        ////////////////////////////////////////////////////////////////////////////////
        // tick
        env.tick_ = 0;

        ////////////////////////////////////////////////////////////////////////////////
        // GLFW
        glfwTerminate();

    }

    env.init( false );
}



} // namespace env


