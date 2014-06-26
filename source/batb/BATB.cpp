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

namespace batb
{



BATB::BATB(const std::string& path) : log( *this ), values( *this ), xml( *this ), keys( *this ),
                                      gui( *this ), forest( *this ), race( *this ), run( *this );
{

    filepath_ = path;

    values.filepath(    file::directory( path ) + "/values/Values.xml" );
    gui.filepath(       file::directory( path ) + "/gui/GUI.xml" );
    forest.filepath(    file::directory( path ) + "/forest/Forest.xml" );
    race.filepath(      file::directory( path ) + "/race/Race.xml" );
    run.filepath(       file::directory( path ) + "/run/Run.xml" );
    //.filepath(          file::directory( path ) + ".xml" );
    
    
}


// initialize BATB-object
void begin(BATB& batb)
{

    // logging
    log::begin( batb.log );

    // general values to use 
    values::begin( batb.values );

    // xml facility
    xml::begin( batb.xml );


    // set up this BATB object from XML
    xml::Document  xml;
    xml::Error err = xml.LoadFile( batb.filepath_.c_str() );
    if ( err == xml::XML_ERROR_FILE_NOT_FOUND )
    {
        std::ostringstream os;
        os << THIS_FUNCTION << ": file not found (XML_ERROR_FILE_NOT_FOUND)";
        throw std::runtime_error( os.str() );
    }
    if ( err == xml::XML_ERROR_FILE_COULD_NOT_BE_OPENED )
    {
        std::ostringstream os;
        os << THIS_FUNCTION << ": file could not be opened (XML_ERROR_FILE_COULD_NOT_BE_OPENED)";
        throw std::runtime_error( os.str() );
    }
    if ( err == xml::XML_ERROR_FILE_READ_ERROR )
    {
        std::ostringstream os;
        os << THIS_FUNCTION << ": file could not be read  (XML_ERROR_FILE_READ_ERROR)";
        throw std::runtime_error( os.str() );
    }
    if ( err == xml::XML_ERROR_EMPTY_DOCUMENT )
    {
        std::ostringstream os;
        os << THIS_FUNCTION << "file is empty (XML_ERROR_EMPTY_DOCUMENT)";
        throw std::runtime_error( os.str() );
    }
    if ( err != xml::XML_NO_ERROR )
    {
        const char* str1 = err.GetErrorStr1();
        const char* str2 = err.GetErrorStr2();
        std::ostringstream os;
        os << THIS_FUNCTION << ": file parsing error";
        if ( str1 ) os << ": " << str1;
        if ( str2 ) os << ", " << str2;
        throw std::runtime_error( os.str() );
    }

    // TODO...
    


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
        saveXML();
        
        // (the non-core part, created by iterationRunBegin, 
        // are shut down by 'iterationRunEnd')


        gui::end( batb.gui );

        keys::end( batb.keys );

        xml::end( batb.xml );

        values::end( batb.values );
        
        log::end( batb.log );
    }

    batb.initialized_ = false;


}


void BATB::saveXML()
{
    xml::Document  xml;
    xml::Error err = xml.SaveFile( filepath_.c_str() );
    
    if ( err != XML_NO_ERROR )
    {
        const char* str1 = err.GetErrorStr1();
        const char* str2 = err.GetErrorStr2();
        batb.log << THIS_FUNCTION << ": save error"  
        if ( str1 ) batb.log << ": " << str1;
        if ( str2 ) batb.log << ", " << str2;
        throw std::runtime_error( os.str() );
    }
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
