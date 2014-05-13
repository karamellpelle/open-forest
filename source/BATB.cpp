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
#include "include.hpp"
#include "BATB.hpp"
#include "BATB/Keys.hpp"
#include "BATB/Values.hpp"

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
    using namespace xml;

    // set BATB configuration-object
    config_ = cfg;

    XMLHandle xml( cfg );

    // create log stream
    Log::create( xml.FirstChildElement("Log").ToElement() );


    // print info
    Env::info( log );
    gl_info( log );


    // create values, from file
    Values::create( xml.FirstChildElement("Values").ToElement() );


    // create Run part of BATB
    theRun()->create( xml.FirstChildElement("Run").ToElement() );

    // create Forest part of BATB
    theForest()->create( xml.FirstChildElement("Forest").ToElement() );
    
    // creat
}

void end()
{
    // FIXME: save config


    theForest()->destroy();
    theRun()->destroy();

}



}

