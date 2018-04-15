//    open-forest: an orientering game.
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
#ifndef BATB_OLD_HPP
#define BATB_OLD_HPP
#include "include.hpp"


namespace old
{

////////////////////////////////////////////////////////////////////////////////

void begin();

void iterate();

void end();

////////////////////////////////////////////////////////////////////////////////
// 

void reset_gl();

////////////////////////////////////////////////////////////////////////////////
//  handle old-errors (system exit)
//

void exit(int err);

bool is_exit();

////////////////////////////////////////////////////////////////////////////////

// we need these to port old-BATB into new-BATB
// the display functions drive the whole old-BATB...
typedef void (*DisplayFunc)();

void set_display_func(DisplayFunc );

DisplayFunc get_display_func();

////////////////////////////////////////////////////////////////////////////////
//
std::string file(const char* path);

int getElapsedTime();




} // namespace old



////////////////////////////////////////////////////////////////////////////////
// removing the GLUT-binding of plib that old-BATB used:
//

#ifdef BATB_BUILD_OLD
#include <plib/pu.h>

inline int puGetWindowOLD()
{
    //return glutGetWindow () ;
    return 0;
}

inline void puSetWindowOLD ( int window )
{
    //glutSetWindow ( window ) ; 
    return;
}

inline void puGetWindowSizeOLD ( int *width, int *height )
{
    //*width  = glutGet ( (GLenum) OLD_WINDOW_WIDTH  ) ;
    //*height = glutGet ( (GLenum) OLD_WINDOW_HEIGHT ) ;
    glfwGetFramebufferSize( glfwGetCurrentContext(), width, height );
}

inline void puSetWindowSizeOLD ( int width, int height )
{
    //glutReshapeWindow ( width, height ) ;
    return;
}

inline void puInitOLD()
{
    puSetWindowFuncs ( puGetWindowOLD,
		       puSetWindowOLD,
		       puGetWindowSizeOLD,
		       puSetWindowSizeOLD ) ;
    puRealInit () ;
}


#endif


#endif
