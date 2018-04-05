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
#include "batb/gl.hpp"

namespace batb
{

namespace gl
{


////////////////////////////////////////////////////////////////////////////////
// set initial state
//
// this defines our GL state invariant 
// (or at least some of our invariant state; arrays should probably disabled after calls)
//

void init_state()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    std::cout << "gl::init_state()" << std::endl;

    //  set up our GL-invariants:
    glEnable( GL_MULTISAMPLE );
    glClearColor( 0, 0, 0, 0 );

    // INVARIANT:
    // if a fragment shader outputs value intended to be color, then this color 
    // should be normalized. that is, the RGB coordinates should be multiplied 
    // by A. this means that if a color (r,g,b) has opacity a, then the RGBA 
    // color should be normalized into (a * r, a * g, a * b, a), i.e.
    // "premultiplied alpha"
    glEnable( GL_BLEND );
    glBlendEquationSeparate( GL_FUNC_ADD, 
                             GL_FUNC_ADD );
    glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA,
                         GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LEQUAL ); // FIXME: strict less, because of round off errors?
    glEnable( GL_DEPTH_TEST );
}




////////////////////////////////////////////////////////////////////////////////
//  resetting our GL state between library (Ogre, nanovg, turbobadger) calls
// 
//  calls have been stripped down. see commit 659cfa97f4006370b6fce005badc13f23dcf5d71
//  for the other calls we had before, if there should be some problem. that
//  commit contains the calls that made these libraries work together for the
//  first time.
//
//  for some reason, only nanovg (or nanovg + turbobadger) without Ogre decrease
//  our framerate. if we add Ogre, then the framerate becomes normal (60 fps).
//  I expect this happened when glEnable( GL_MULTISAMPLE ) was added, but
//  it has not been verified.
//
//
//  actually, only gl::end_XXX() should be necessary (shown as 'reset' below)
//
//  current draw order: 
//
//     Ogre     ->      nanovg      ->        turbobadger     ->    ...
//          reset Ogre          reset nanovg                reset tb/init_state
//
//



////////////////////////////////////////////////////////////////////////////////
// working with Ogre 
//
void begin_ogre()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

}

// reset GL state after Ogre
void end_ogre()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    glDisable( GL_LIGHTING );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //glDisableClientState( GL_NORMAL_ARRAY );
    //glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    //glDisableClientState( GL_VERTEX_ARRAY );
    //glDisableClientState( GL_INDEX_ARRAY );
    //glDisableClientState( GL_SECONDARY_COLOR_ARRAY );

    // set premult blend equation?
}

////////////////////////////////////////////////////////////////////////////////
// working with nanovg
//

void begin_nanovg()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

}


// reset GL state after nanovg
void end_nanovg()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    // see nanovg's README.md for altered state
    glEnable( GL_DEPTH_TEST );
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

////////////////////////////////////////////////////////////////////////////////
}


////////////////////////////////////////////////////////////////////////////////
// working with turbobadger
//

void begin_turbobadger()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

}

// reset GL state after turbobadger
// see TBRendererGL::BeginPaint in tb_renderer_gl.cpp
void end_turbobadger()
{
debug::gl::DebugGroup( DEBUG_FUNCTION_NAME );

    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY ); 
    glDisableClientState( GL_VERTEX_ARRAY ); 

    glDisable( GL_TEXTURE_2D );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );

    // blending premultiplied colors
    //glBlendEquationSeparate( GL_FUNC_ADD, 
    //                         GL_FUNC_ADD );
    glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA,
                         GL_ONE, GL_ONE_MINUS_SRC_ALPHA );



}




} //namespace gl

} // namespace batb



