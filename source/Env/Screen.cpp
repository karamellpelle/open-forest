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


void screenBegin(xml::XMLElement* elem)
{
    using namespace xml;

    XMLHandle xml( elem );

    // set hints to window
    // http://www.glfw.org/docs/latest/window.html#window_hints

    // set default first. (not necessary done automatically, according to doc)
    glfwDefaultWindowHints();

    //glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );    :)
    //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_CORE_PROFILE /* GLFW_COMPAT_PROFILE */ ); // OpenGL 3.2+
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_DECORATED, GL_FALSE );

    // FIXME: parse hints after above hints

    // default settings, to be overridden by XML
    uint wth = 640;
    uint hth = 480;
    GLFWmonitor* monitor = 0;
    uint samples = 0;


    // size
    XMLElement* xml_size_wth = xml.FirstChildElement("size").FirstChildElement("wth").ToElement();
    XMLElement* xml_size_hth = xml.FirstChildElement("size").FirstChildElement("hth").ToElement();
    if ( xml_size_wth && xml_size_hth )
    {
        unsigned int w = wth;
        unsigned int h = hth;
        xml_size_wth->QueryUnsignedText( &w );
        xml_size_wth->QueryUnsignedText( &h );
        wth = w;
        hth = h;
    }

    // fullscreen?
    XMLElement* xml_fullscreen = xml.FirstChildElement("fullscreen").ToElement();
    if ( xml_fullscreen )
    {
        bool fullscreen = false;
        xml_fullscreen->QueryBoolText( &fullscreen );
        monitor = fullscreen ? glfwGetPrimaryMonitor() : 0;
    }

    XMLElement* xml_multisamples = xml.FirstChildElement("multisamples").ToElement();
    if ( xml_multisamples )
    {
        unsigned int n = samples;
        xml_multisamples->QueryUnsignedText( &n );
        samples = n;
    }


    // set MSAA samples
    glfwWindowHint( GLFW_SAMPLES, samples ); 

    theWindow_ = glfwCreateWindow( wth, hth, "OpenForest", monitor, 0 );

    // set GL context as 'theWindow_'
    glfwMakeContextCurrent( theWindow_ );

    if ( !theWindow_ )
    {
        throw std::runtime_error( "could not create window" );
    }


    // we now have a context, init GLEW
    GLenum err = glewInit();
    if ( err != GLEW_OK )
    {
        std::ostringstream os;
        os << "Env: ERROR could not init GLEW, " << glewGetErrorString( err );
        throw std::runtime_error( os.str() );
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
