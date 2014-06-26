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
#include "Env.hpp"

namespace Env
{

static xml::XMLElement* theXMLElement_;



static void glfw_error_callback(int error, const char* str)
{
    std::cerr << "Env : ERROR could not init GLFW, \"" << str << " (code " << error << ")\" " << std::endl;
}


// XMLElement must be valid through Env whole lifetime
void begin(xml::XMLElement* elem)
{

    // set the XMLElement for Env
    theXMLElement_ = elem;

    xml::XMLHandle xml( elem );


    // cfg GLFW
    glfwSetErrorCallback( glfw_error_callback );
    if ( !glfwInit() )
    {
        throw std::runtime_error( "Env: could not init GLFW" );
    }

    try
    {
        // create Screen
        screenBegin( xml.FirstChildElement( "Screen" ).ToElement() );

        // create Sound
        // ...
    }
    catch (std::exception& e)
    {
        std::ostringstream os;
        os << "Env : could not init: " << e.what(); 
        throw std::runtime_error( os.str() );
    }
}

void end()
{

    // soundEnd();
    //
    screenEnd();

    // end GLFW
    glfwTerminate();
}

// print Env info
void info(std::ostream& os)
{
    screenInfo( os );
}

xml::XMLElement* xmlElement()
{
    return theXMLElement_;
}

}
