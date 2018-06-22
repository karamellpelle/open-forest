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
        nvg_context = nvgCreateGL2( nanovg_flags );
	if ( nvg_context == nullptr )
        {
            batb->log << "ERROR: could not create NanoVG GL2 context" << std::endl;
            throw std::runtime_error( "GL: could not create NanoVG GL2 context" );
	}
        batb->log << "NanoVG GL2 context created" << std::endl;
#endif
#ifdef NANOVG_GL3_IMPLEMENTATION
        nvg_context = nvgCreateGL3( nanovg_flags ); 
	if ( nvg_context == nullptr )
        {
            batb->log << "ERROR: could not create NanoVG GL3 context" << std::endl;
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

debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );


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
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );


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

    glEnable( GL_DEPTH_TEST );
    glEnable(GL_BLEND);

    // this is important with respect to Ogre: do not bind the program
    // after use. otherwise it breaks Ogre's ProgramPipelineObject 
    // see https://www.khronos.org/opengl/wiki/Shader_Compilation#Separate_programs
    //glUseProgram( 0 );
    // ^ NOTE: done by 'TBRendererGL330::EndPaint()' instead, as the NanoVG lib does
    // for us
}


////////////////////////////////////////////////////////////////////////////////
// working with nanovg


NVGcontext* GL::nanovgBegin(const Scene& scene)
{
    // NanoVG only likes pixel coordinates (which differs from fragments: the 
    // primitive size of Scene)

    // calculate pixel ration for hi-dpi targets
    float px_ratio = (float)(scene.wth) / (float)(scene.wth_px);

    nvgBeginFrame( nvg_context, scene.wth_px, scene.hth_px, px_ratio );

    return nvg_context;
}



void GL::nanovgEnd()
{
debug::gl::DebugGroup _dbg( DEBUG_FUNCTION_NAME );

    nvgEndFrame( nvg_context );
    // ^ calls glUseProgram( 0 ), see glnvg__renderFlush() in nanovg_gl.h

    // TODO: can we remove this?
    // see https://github.com/memononen/nanovg#opengl-state-touched-by-the-backend
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
