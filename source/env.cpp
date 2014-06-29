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
#include "env.hpp"

namespace env
{

xml::XMLDocument doc_;



static void glfw_error_callback(int error, const char* str)
{
    std::cerr << "env : ERROR: could not init GLFW, \"" << str << " (code " << error << ")\" " << std::endl;
}


void begin(const std::string& filepath)
{
    using namespace xml;

    // load our global 'doc_' variable
    XMLError err = doc_.LoadFile( filepath.c_str() );
    if ( err == XML_ERROR_FILE_NOT_FOUND )
    {
        std::ostringstream os;
        os << "env : ERROR: file not found, " << filepath << " (XML_ERROR_FILE_NOT_FOUND)";
        throw std::runtime_error( os.str() );
    }
    if ( err == XML_ERROR_FILE_COULD_NOT_BE_OPENED )
    {
        std::ostringstream os;
        os << "env : ERROR: file could not be opened (XML_ERROR_FILE_COULD_NOT_BE_OPENED)";
        throw std::runtime_error( os.str() );
    }
    if ( err == XML_ERROR_FILE_READ_ERROR )
    {
        std::ostringstream os;
        os << "env : ERROR: file could not be read  (XML_ERROR_FILE_READ_ERROR)";
        throw std::runtime_error( os.str() );
    }
    if ( err == XML_ERROR_EMPTY_DOCUMENT )
    {
        std::ostringstream os;
        os << "env : ERROR: file is empty (XML_ERROR_EMPTY_DOCUMENT)";
        throw std::runtime_error( os.str() );
    }
    if ( err != XML_NO_ERROR )
    {
        const char* str1 = doc_.GetErrorStr1();
        const char* str2 = doc_.GetErrorStr2();
        std::ostringstream os;
        os << "env : ERROR: file parsing error";
        if ( str1 ) os << ": " << str1;
        if ( str2 ) os << ", " << str2;

        throw std::runtime_error( os.str() );
    }

    // cfg GLFW
    glfwSetErrorCallback( glfw_error_callback );
    if ( !glfwInit() )
    {
        throw std::runtime_error( "env : ERROR: could not init GLFW" );
    }

    // create Screen
    screen_begin_();

    // create Sound
    // ...
}

void end()
{

    // soundEnd();
    //
    screen_end_();

    // end GLFW
    glfwTerminate();
}

} // namespace env
