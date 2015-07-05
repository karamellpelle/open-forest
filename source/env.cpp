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
#include "env.hpp"

namespace env
{

YAML::Node doc_;



static void glfw_error_callback(int error, const char* str)
{
    std::cerr << "env : GLFW error: \"" << str << " (code " << error << ")\" " << std::endl;
}


void begin(const std::string& filepath)
{
    // load our global 'doc_' variable (configuration)
    doc_ = YAML::LoadFile( filepath );

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
