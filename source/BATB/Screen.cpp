//    open-forest: an orienteering game.
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
#include "BATB/Screen.hpp"
#include "BATB/Time.hpp"

extern "C"
{
static void glfw_error_callback(int error, const char* str)
{
    // FIXME: use 'batb' object!
    std::cout << "GLFW error (code " << error << "): " << str << std::endl;
}
}


namespace batb
{
namespace screen
{


////////////////////////////////////////////////////////////////////////////////
// setup

void Screen::begin(const std::string& path)
{

    if ( init_empty() )
    {
        // set configuration file
        config( path );

        ////////////////////////////////////////////////////////////////////////////////
        // setup GLFW
        //
        glfwSetErrorCallback( glfw_error_callback );

        if ( !glfwInit() )
        {
            throw std::runtime_error( "could not init GLFW" );
        }

        ////////////////////////////////////////////////////////////////////////////////
        // screen
        // TODO: print GL info if verbose setting in yaml


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
        bool decorate = yaml["decorate"].as<bool>( false );
        glfwWindowHint( GLFW_DECORATED, decorate );               

        // size
        uint wth = 640;
        uint hth = 480;
        if ( YAML::Node node = yaml[ "size" ] )
        {
            wth = node[ "wth" ].as<uint>( wth );
            hth = node[ "hth" ].as<uint>( hth );
        }

        // fullscreen
        bool fullscreen = false;
        if ( YAML::Node node = yaml[ "fullscreen" ] )
        {
            glfw_monitor = node.as<bool>( fullscreen ) ? glfwGetPrimaryMonitor() : 0;
        }

        // multisamples
        uint samples = 0;
        if ( YAML::Node node = yaml[ "multisamples" ] )
        {
            samples = node.as<uint>( samples );
            glfwWindowHint( GLFW_SAMPLES, samples ); 
        }

        // title
        std::string title = "";
        if ( YAML::Node node = yaml[ "title" ] )
        {
            title = node.as<std::string>( title );
        }

        
        // NOTE: error in implementation of glfw, according to valgrind:
        glfw_window = glfwCreateWindow( wth, hth, title.c_str(), glfw_monitor, 0 );

        // set GL context as 'theWindow_'
        glfwMakeContextCurrent( glfw_window );

        if ( !glfw_window )
        {
            throw std::runtime_error( "env::begin: could not create glfw_window" );
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

    init( true );
}

void Screen::end()
{
    if ( init_nonempty() )
    {
        save();

        ////////////////////////////////////////////////////////////////////////////////
        // screen
        glfwDestroyWindow( glfw_window ); 
        glfw_window = nullptr;
        glfw_monitor = nullptr;
        fbo_ = 0;
        fbo0_ = 0;
        fbo1_ = 0;

        ////////////////////////////////////////////////////////////////////////////////
        // frame
        frame_count_ = 0;
        frame_fps_ = 0.0;

        ////////////////////////////////////////////////////////////////////////////////
        // GLFW
        glfwTerminate();

    }

    init( false );
}

////////////////////////////////////////////////////////////////////////////////
// frame
//

void Screen::frameBegin()
{
    auto tick = batb->time->get();

    // compute frames per second
    constexpr tick_t update_delta = 0.4; // update interval in seconds
    double delta = tick - frame_fps_tick_;
    if ( update_delta <= delta )
    {
        frame_fps_ = (frame_count_ - frame_fps_count_) / delta;

        frame_fps_tick_ = tick; 
        frame_fps_count_ = frame_count_;
    }

    // populate events
    glfwPollEvents();
}

void Screen::frameEnd()
{
    glfwSwapBuffers( glfw_window );

    ++frame_count_;

}

////////////////////////////////////////////////////////////////////////////////
// screen
//
// TODO: helper FBO (fbo1), change size on window resize

void Screen::getSize(uint& wth, uint& hth) const
{
    int w, h;
    glfwGetFramebufferSize( glfw_window, &w, &h );
    wth = w;
    hth = h;
}

void Screen::getShape(float_t& wth, float_t& hth) const
{
    uint w, h;
    getSize( w, h );

    float_t scale = 1.0 / std::max( w, h );
    wth = (float_t)( w ) * scale;
    hth = (float_t)( h ) * scale;
}

GLuint Screen::setFBO(GLuint fbo)
{
    GLuint ret = fbo_;

    // TODO: bind 'fbo'
    batb->log << DEBUG_FUNCTION_NAME << ": TODO: FBO not implemented." << std::endl;
    fbo_ = fbo;

    return ret;
}



} // namespace screen
} // namespace batb


