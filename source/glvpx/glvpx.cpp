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
#include "glvpx/glvpx.hpp"

namespace glvpx
{

file::file(const char* path)
{

}

frame::frame(const file& f)
{
    // create tex render into
    glGenTextures( 1, &tex_ );
    glBindTexture( GL_TEXTURE_2D, tex );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_FILTER, GL_LINEAR );
  
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, f.wth_, f.hth_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );
}

bool render_frame(double tick, frame& f)
{
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

}

void frame_present_std(const frame& f)
{

}


}

