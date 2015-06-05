//    open-forest: an orienteering game.
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
#include "batb/gl.hpp"

namespace batb
{

namespace gl
{

////////////////////////////////////////////////////////////////////////////////
// GL state
//
// current draw order: 
//
//    Ogre     ->      nanovg      ->        turbobadger     ->    ...
//         reset_Ogre          reset_nanovg                reset
//
//


// set initial state
void reset()
{
    glDepthFunc( GL_LEQUAL ); 
/*
    //  set up our GL-invariants:
    glEnable( GL_MULTISAMPLE );
    glClearColor( 0, 0, 0, 0 );
    glDisable( GL_STENCIL_TEST ); // ??
    glClearStencil( 0 );          // ??
    std::printf("glEnable: %p\n", glEnable);
    std::printf("glBlendFuncSeparate: %p\n", glBlendFuncSeparate);
    // INVARIANT:
    // if a fragment shader outputs value intended to be color, then this color should be normalized.
    // that is, the RGB coordinates should be multiplied by A. this means that if a color (r,g,b) has
    // opacity a, then the RGBA color should be normalized into (a * r, a * g, a * b, a).
    // "premultiplied alpha"
    glEnable( GL_BLEND );
    glBlendEquationSeparate( GL_FUNC_ADD, 
                             GL_FUNC_ADD );
    glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA,
                         GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LEQUAL ); // FIXME: strict less, because of round off errors?
    glEnable( GL_DEPTH_TEST );
*/
}

// reset GL state after Ogre
void reset_Ogre()
{

}

// reset GL state after turbobadger
void reset_turbobadger()
{

}

// reset GL state after nanovg
void reset_nanovg()
{
    // see old.cpp and README.md for nanovg
    glEnable( GL_DEPTH_TEST );
    glBindBuffer(GL_ARRAY_BUFFER, 0); // this seems to be the problem if no other output that nanovg...

}


} //namespace gl

} // namespace batb



