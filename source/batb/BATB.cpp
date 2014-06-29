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
#include "batb/BATB.hpp"

namespace batb
{



BATB::BATB(const std::string& path) : log( *this ), xml( *this), value( *this ),  
                                      keys( *this ), gui( *this )
                                      //forest( *this ), race( *this ), run( *this );
{

    filepath_ = path;

    // core:
    value.filepath(     file::directory( path ) + "/value/Value.xml" );
    gui.filepath(       file::directory( path ) + "/gui/GUI.xml" );

    // non-core:
    //forest.filepath(    file::directory( path ) + "/forest/Forest.xml" );
    //race.filepath(      file::directory( path ) + "/race/Race.xml" );
    //run.filepath(       file::directory( path ) + "/run/Run.xml" );
    //.filepath(          file::directory( path ) + ".xml" );
    
    
}


// initialize BATB-object
void begin(BATB& batb)
{

    // logging
    log::begin( batb.log );

    // xml facility
    xml::begin( batb.xml );

    // general values to use 
    value::begin( batb.value );


    // set up this BATB object from XML
    xml::Document doc;
    std::string errstr;
    if ( auto err = xml::load_document( doc, batb.filepath_.c_str(), THIS_FUNCTION, errstr ) )
    {
        throw std::runtime_error( errstr );
    }

    // now parse document
    // ...
    


    //////////////////////////////////////////////////////////
    //      OpenGL
    // BATB assumes this GL state:
    //set_gl_state();


    //////////////////////////////////////////////////////////
    //      OpenAL



    // keys
    keys::begin( batb.keys );

    // gui
    gui::begin( batb.gui );

    // (now the rest of BATB (forest, race, run, ...) are loaded by iterationRunMain)

    
    batb.initialized_ = true;
}

void end(BATB& batb)
{
    if ( batb.initialized_ )
    {
        // save the configuration to its XML file
        batb.saveXML();
        
        // (the non-core part, created by iterationRunBegin, 
        // are shut down by 'iterationRunEnd')


        gui::end( batb.gui );

        keys::end( batb.keys );

        xml::end( batb.xml );

        value::end( batb.value );
        
        log::end( batb.log );
    }

    batb.initialized_ = false;


}


void BATB::saveXML()
{
    xml::Document doc;

    // FIXME: populate
// ignore for now...
/*
    std::string errstr;
    if ( auto err = xml::save_document( doc, filepath_, THIS_FUNCTION, errstr ) )
    {
        log << errstr << std::endl;
    }
*/
}

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
