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
#ifndef BATB_DEBUG_GL_HPP
#define BATB_DEBUG_GL_HPP
#include <GL/glew.h>

namespace debug
{

namespace gl
{

////////////////////////////////////////////////////////////////////////////////
// these are handy together with https://github.com/apitrace/apitrace
// see https://www.khronos.org/opengl/wiki/Debug_Output



inline void gl_push_group(const std::string& name)
{
    if (GLEW_KHR_debug)
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name.c_str() );
    }
}

inline void gl_pop_group()
{

    if (GLEW_KHR_debug)
    {
        glPopDebugGroup();
    }
}

inline void msg(const std::string& msg)
{
    if (GLEW_KHR_debug)
    {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER,
                         0, GL_DEBUG_SEVERITY_NOTIFICATION, -1, msg.c_str() );
    }

}


class DebugGroup
{
public:
    DebugGroup(const std::string& name) { gl_push_group( name ); }
    ~DebugGroup() { gl_pop_group(); }
};


} // namespace gl

} // namespace debug


#endif

