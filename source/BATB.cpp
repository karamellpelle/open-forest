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
#include "BATB.hpp"
#include "BATB/Keys.hpp"
#include "BATB/Values.hpp"
#include "BATB/GUI.hpp"

namespace BATB
{

static Config* config_;

static void gl_info(std::ostream& os)
{
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    //const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    //const GLubyte* GLUversion = gluGetString(GLU_VERSION);
    
    os << "OpenGL: " << "\n";
    os << "  vendor:             " << vendor << "\n";
    os << "  renderer:           " << renderer << "\n";
    os << "  GL version:         " << version << "\n";
#ifdef GL_SHADING_LANGUAGE_VERSION
    const GLubyte* sl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (sl_version != 0) {
    os << "  GLSL version:       " << sl_version << "\n";
    }
#endif
#ifdef GL_MAX_TEXTURE_UNITS
    GLint maxTextureUnits[1];
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, maxTextureUnits);
    os << "  max texture units:  " << maxTextureUnits[0] << "\n";
#endif
    //os << "Extensions: " << extensions << "\n";
    //os << "GLU Version: " << GLUversion << std::endl;
    os << std::flush;
}

void begin(Config* cfg)
{

    // set our BATB configuration-object
    config_ = cfg;


    using namespace xml;
    XMLHandle xml( cfg );

    // create our values, using our Config'uration
    Values::create( xml.FirstChildElement( "Values" ).ToElement() );

    // create log stream
    Log::create( xml.FirstChildElement("Log").ToElement() );



    //////////////////////////////////////////////////////////
    //      OpenGL
    // BATB assumes this GL state:
    //set_gl_state();


    //////////////////////////////////////////////////////////
    //      OpenAL



    // print info
    Env::info( log );
    gl_info( log );


    // create values, from file
    Values::create( xml.FirstChildElement("Values").ToElement() );

    //
    theGUI()->create( xml.FirstChildElement("GUI").ToElement());

    // create Forest part of BATB
    theForest()->create( xml.FirstChildElement("Forest").ToElement() );

    // create Race part of BATB
    // FIXME!

    // create Run part of BATB
    theRun()->create( xml.FirstChildElement("Run").ToElement() );

    
    // creat
}

void end()
{

    // FIXME: save config

    theGUI()->destroy();

    theRun()->destroy();
    theForest()->destroy();

}


void set_gl_state()
{
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
}

}
