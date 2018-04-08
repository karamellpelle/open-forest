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
#include "batb.hpp"
#include "batb/gl/GL.hpp"
#include "batb/Scene.hpp"

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

    BATB_LOG_FUNC( gl.batb );
    
    if ( init_empty() )
    {
        // set configuration file
        config( path );

        // init our GL state
        gl::init_state();

#ifdef NANOVG_GL2_IMPLEMENTATION
        nvg_context = nvgCreateGL2( NVG_STENCIL_STROKES | NVG_DEBUG );
#endif
#ifdef NANOVG_GL3_IMPLEMENTATION

#endif
	if ( nvg_context == nullptr )
        {
            throw std::runtime_error( "GL: could not create nanovg context" );
	}

        
    }
    ////////////////////////////////////////////////////////////////////////////////
    
    init( true );


    
}


void GL::end()
{
    BATB_LOG_FUNC( gl.batb );

    if ( init_nonempty() )
    {
         //clear fonts
        //for ( auto& i : gl.nanovg_fonts_ )
        //{
        //    // (release font not necessary (?))
        //
        //}
#ifdef NANOVG_GL2_IMPLEMENTATION
        nvgDeleteGL2( nvg_context );
#endif
#ifdef NANOVG_GL3_IMPLEMENTATION

#endif
    }
   
    init( false );
}

////////////////////////////////////////////////////////////////////////////////
// working with nanovg

NVGcontext* GL::nanovg_begin(const Scene& scene)
{
    gl::begin_nanovg();

    // TODO: use Scene!!!
    int winWidth, winHeight;
    glfwGetWindowSize( batb->env->window, &winWidth, &winHeight );
    int fbWidth, fbHeight;
    glfwGetFramebufferSize( batb->env->window, &fbWidth, &fbHeight ); // FIXME: remove env
    float pxRatio = (float)fbWidth / (float)winWidth; // Calculate pixel ration for hi-dpi devices.

    auto wth = scene.wth;
    auto hth = scene.hth;
    nvgBeginFrame( nvg_context, wth, hth, pxRatio);
    //nvgSave( nvg );

    return nvg_context;
}

void GL::nanovg_end()
{
    //nvgRestore( nvg );
    nvgEndFrame( nvg_context );
    gl::end_nanovg();

}

void GL::nanovg_normalize(const Scene& scene)
{
    //float_t s = std::max( scene.wth, scene.hth );
    float_t s = scene.wth; // normalize relative to width
    nvgScale( nvg_context, s, s );
}

int GL::nanovg_font(const std::string& name, const std::string& file)
{
    auto h =  nvgCreateFont( nvg_context, name.c_str(), file.c_str() );
    //nanovg_fonts_[ name ] = h;
    return h;
}


} // namespace gl

} // namespace batb
