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
#include "env/screen.hpp"


namespace env
{


GLFWwindow* screen_window_ = nullptr;



void screen_begin_()
{
    // TODO: print GL info if verbose setting in config

    GLFWmonitor* monitor = nullptr;

    // default settings, to be overridden by yaml 
    uint wth = 640;
    uint hth = 480;
    bool fullscreen = false;
    uint samples = 0;
    std::string title = "";


    YAML::Node yaml = doc_[ "screen" ];

    // set hints to window
    // http://www.glfw.org/docs/latest/window.html#window_hints

    // set default first. (not necessary done automatically, according to doc)
    glfwDefaultWindowHints();

    //glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );    :)
    //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_CORE_PROFILE /* GLFW_COMPAT_PROFILE */ ); // OpenGL 3.2+
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );    
    glfwWindowHint( GLFW_DECORATED, GL_FALSE ); // window close functionality is not implemented anyway

    // FIXME: parse hints after above hints


    // size
    {
        YAML::Node node = yaml[ "size" ];
        wth = node[ "wth" ].as<uint>( wth );
        hth = node[ "hth" ].as<uint>( hth );
    }

    // fullscreen
    {
        YAML::Node node = yaml[ "fullscreen" ];
        monitor = node.as<bool>( fullscreen ) ? glfwGetPrimaryMonitor() : 0;
    }

    // multisamples
    {
        YAML::Node node = yaml[ "multisamples" ];
        samples = node.as<uint>( samples );

        // set MSAA samples
        glfwWindowHint( GLFW_SAMPLES, samples ); 
    }

    // title
    {
        YAML::Node node = yaml[ "title" ];
        title = node.as<std::string>( title );
    }

    
    screen_window_ = glfwCreateWindow( wth, hth, title.c_str(), monitor, 0 );

    // set GL context as 'theWindow_'
    glfwMakeContextCurrent( screen_window_ );

    if ( !screen_window_ )
    {
        throw std::runtime_error( "env : ERROR: could not create window" );
    }


    // we now have a context, init GLEW
    GLenum err = glewInit();
    if ( err != GLEW_OK )
    {
        std::ostringstream os;
        os << "env : ERROR: could not init GLEW, " << glewGetErrorString( err );
        throw std::runtime_error( os.str() );
    }

}


void screen_end_()
{
    glfwDestroyWindow( screen_window_ ); 

    screen_window_ = nullptr;
}


}
