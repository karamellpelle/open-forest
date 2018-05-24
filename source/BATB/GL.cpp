//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU Genergl Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Genergl Public License for more details.
//
//    You should have received a copy of the GNU Genergl Public License glong
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "BATB/GL.hpp"
#include "BATB/Scene.hpp"
#include "BATB/Screen.hpp"

// include implementation of nanovg code
#include "nanovg_gl.h"


namespace batb
{



namespace gl
{


////////////////////////////////////////////////////////////////////////////////
// setup 
//

void GL::begin(const std::string& path)
{

    batb->log << "batb->gl->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
        // set configuration file
        config( path );

        // init our GL state
        initState();
        batb->log << "default state initialized" << std::endl;

        ////////////////////////////////////////////////////////////////
        // NanoVG
        // 

        // context creation flags
        int nanovg_flags = NVG_STENCIL_STROKES;

        // add debug to nanovg?
        if ( YAML::Node node = yaml["nanovg-debug"] )
        {
            nanovg_flags |= (node.as<bool>( false ) ? NVG_DEBUG : 0);
        }

#ifdef NANOVG_GL2_IMPLEMENTATION
        nvg_context = nvgCreateGL2( nanovg_flags ); // TODO: add debug if defined in yaml
	if ( nvg_context == nullptr )
        {
            batb->log << "ERROR: could not create NanoVG GL2 context" << std::endl;
            batb->log->indentPop();
            throw std::runtime_error( "GL: could not create NanoVG GL2 context" );
	}
        batb->log << "NanoVG GL2 context created" << std::endl;
#endif
#ifdef NANOVG_GL3_IMPLEMENTATION
        nvg_context = nvgCreateGL3( nanovg_flags ); 
	if ( nvg_context == nullptr )
        {
            batb->log << "ERROR: could not create NanoVG GL3 context" << std::endl;
            batb->log->indentPop();
            throw std::runtime_error( "GL: could not create NanoVG GL3 context" );
	}
        batb->log << "NanoVG GL3 context created" << std::endl;
#endif

        ////////////////////////////////////////////////////////////////
        //

        
    }
    ////////////////////////////////////////////////////////////////////////////////
    
    init( true );

    
}


void GL::end()
{
    batb->log << "batb->gl->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        save();

        //clear nanovg fonts?
        //for ( auto& i : gl.nanovg_fonts_ )
        //{
        //    // (release font not necessary (?))
        //
        //}

#ifdef NANOVG_GL2_IMPLEMENTATION
        nvgDeleteGL2( nvg_context );
        batb->log << "NanoVG GL2 context deleted" << std::endl;
#endif
#ifdef NANOVG_GL3_IMPLEMENTATION
        nvgDeleteGL3( nvg_context );
        batb->log << "NanoVG GL3 context deleted" << std::endl;
#endif

        nvg_context = nullptr;

    }
   
    init( false );

}

////////////////////////////////////////////////////////////////////////////////
// set initial state
//
// this defines our GL state invariant 
// (or at least some of our invariant state; arrays should probably disabled after calls)
//

void GL::initState()
{
#if 0
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    //batb->log << DEBUG_FUNCTION_NAME << std::endl;

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
#endif
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
void GL::ogreBegin()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

}

// reset GL state after Ogre
void GL::ogreEnd()
{
#if 0
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    glDisable( GL_LIGHTING );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    //glDisableClientState( GL_NORMAL_ARRAY );
    //glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    //glDisableClientState( GL_VERTEX_ARRAY );
    //glDisableClientState( GL_INDEX_ARRAY );
    //glDisableClientState( GL_SECONDARY_COLOR_ARRAY );

    // set premult blend equation?
#endif
}


////////////////////////////////////////////////////////////////////////////////
// working with turbobadger
//

void GL::turbobadgerBegin()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

}

// reset GL state after turbobadger
// see the TBRenderer's (i.e. TBRendererGL330) BeginPaint()/EndPaint()
void GL::turbobadgerEnd()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    //glDisableClientState( GL_COLOR_ARRAY );
    //glDisableClientState( GL_TEXTURE_COORD_ARRAY ); 
    //glDisableClientState( GL_VERTEX_ARRAY ); 
    //glDisable( GL_TEXTURE_2D );

    //glDepthFunc( GL_LEQUAL );
    //glEnable( GL_DEPTH_TEST );
    //glEnable(GL_BLEND);

    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(2);

    // blending premultiplied colors
    glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA,
                         GL_ONE, GL_ONE_MINUS_SRC_ALPHA );



}


////////////////////////////////////////////////////////////////////////////////
// working with nanovg

void GL::nanovgBegin()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

}


NVGcontext* GL::nanovgBegin(const Scene& scene)
{

    // calculate pixel ration for hi-dpi devices.
    int winWidth, winHeight;
    glfwGetWindowSize( batb->screen->glfw_window, &winWidth, &winHeight ); 
    int fbWidth, fbHeight; 
    glfwGetFramebufferSize( batb->screen->glfw_window, &fbWidth, &fbHeight ); 
    float pxRatio = (float)fbWidth / (float)winWidth; 

    auto wth = scene.wth;
    auto hth = scene.hth;
    nvgBeginFrame( nvg_context, wth, hth, pxRatio);
    //nvgSave( nvg );

    return nvg_context;

}

void GL::nanovgEnd()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    //nvgRestore( nvg );
    nvgEndFrame( nvg_context );

    // see
    // https://github.com/memononen/nanovg#opengl-state-touched-by-the-backend
    //glUseProgram(prog);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glEnable(GL_BLEND);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_SCISSOR_TEST);
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glStencilMask(0xffffffff);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    //glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
    //glActiveTexture(GL_TEXTURE0);
    //glBindBuffer(GL_UNIFORM_BUFFER, buf);
    //glBindVertexArray(arr);
    //glBindBuffer(GL_ARRAY_BUFFER, buf);
    //glBindTexture(GL_TEXTURE_2D, tex);
    //glUniformBlockBinding(... , GLNVG_FRAG_BINDING);

    glEnable( GL_DEPTH_TEST );
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //glBindVertexArray(arr);
    glBlendEquationSeparate( GL_FUNC_ADD, 
                             GL_FUNC_ADD );
    glBlendFuncSeparate( GL_ONE, GL_ONE_MINUS_SRC_ALPHA,
                         GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
////////////////////////////////////////////////////////////////////////////////

}

void GL::nanovgNormalize(const Scene& scene)
{
    //float_t s = std::max( scene.wth, scene.hth );
    float_t s = scene.wth; // normalize relative to width
    nvgScale( nvg_context, s, s );
}

int GL::nanovgFont(const std::string& name, const std::string& file)
{
    auto h =  nvgCreateFont( nvg_context, name.c_str(), file.c_str() );
    //nanovg_fonts_[ name ] = h;
    return h;
}


} // namespace gl

} // namespace batb
