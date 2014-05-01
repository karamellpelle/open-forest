//    orienteering-game: an orientering game.
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
#include "Env/Screen.hpp"


namespace Env
{


GLFWwindow* theWindow_ = 0;


void screenBegin(Config::Block* )
{
    // set hints to window
    // http://www.glfw.org/docs/latest/window.html#window_hints
    //glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
    //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_CORE_PROFILE /* GLFW_COMPAT_PROFILE */ ); // OpenGL 3.2+
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_DECORATED, GL_FALSE );
    glfwWindowHint( GLFW_SAMPLES, 4 ); // 4 samples for multisampling

    // fullscreen
    //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    // window 
    GLFWmonitor* monitor = 0;

    theWindow_ = glfwCreateWindow( 640, 480, "GLFW screenBegin", monitor, 0 );

    // set GL context as 'theWindow_'
    glfwMakeContextCurrent( theWindow_ );

    if ( !theWindow_ )
    {
        throw std::runtime_error( "could not create window" );
    }
}


void screenEnd()
{
    glfwDestroyWindow( theWindow_ ); 
}


void screenInfo(std::ostream& os)
{
    // TODO: print GLFW window info
}

}
