//    open-forest: an orientering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
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
#include "batb/Scene.hpp"


namespace batb
{



namespace gl
{


////////////////////////////////////////////////////////////////////////////////
//  GL



////////////////////////////////////////////////////////////////////////////////
// 
void begin(GL& gl)
{

    BATB_LOG_FUNC( gl.batb );
    
    if ( gl.init_empty() )
    {

        // init our GL state
        gl::init_state();

#ifdef NANOVG_GL2_IMPLEMENTATION
        gl.nvg_context = nvgCreateGL2(512, 512, 0);
#endif
#ifdef NANOVG_GL3_IMPLEMENTATION
	gl.nvg_context = nvgCreateGL3(512, 512, 0);
#endif
	if ( gl.nvg_context == nullptr )
        {
            throw std::runtime_error( "GL: could not create nanobatb.nvg_context context" );
	}

        
    }
    ////////////////////////////////////////////////////////////////////////////////
    
    gl.init( true );


    
}


void end(GL& gl)
{
    BATB_LOG_FUNC( gl.batb );

    if ( gl.init_nonempty() )
    {
        nvgDeleteGL2(gl.nvg_context);
    }
   
    gl.init( false );
}

} // namespace gl

} // namespace batb
