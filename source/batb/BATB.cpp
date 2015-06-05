//    open-forest: an orientering game.
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



BATB::BATB(const std::string& path) : log( *this ), value( *this ),  
                                      keys( *this ), gui( *this ),
                                      ogre( *this ),
                                      //forest( *this ), race( *this ), run( *this );
                                      run( *this )
{

    filepath_ = path;

    // core:
    value.filepath(     file::directory( path ) + "/value/Value.yaml" );
    gui.filepath(       file::directory( path ) + "/gui/GUI.yaml" );

    // non-core:
    ogre.filepath(      file::directory( path ) + "/ogre/OGRE.yaml" );
    //forest.filepath(    file::directory( path ) + "/forest/Forest.yaml" );
    //race.filepath(      file::directory( path ) + "/race/Race.yaml" );
    run.filepath(       file::directory( path ) + "/run/Run.yaml" );
    //.filepath(          file::directory( path ) + ".yaml" );
    
    
}


// initialize BATB and its core parts
void begin(BATB& batb)
{

    // logging
    log::begin( batb.log );

    // general values to use 
    value::begin( batb.value );


    // set up this BATB object from file
    YAML::Node yaml = YAML::LoadFile( batb.filepath_ );
    // now parse document
    // ...
    


    //////////////////////////////////////////////////////////
    //      OpenGL
    // BATB assumes this GL state:
    gl::reset();


    //////////////////////////////////////////////////////////
    //      OpenAL



    // keys
    keys::begin( batb.keys );

    // gui
    gui::begin( batb.gui );

    // we need these two core iterations up and running:
    run::begin( batb.run.iterationRunBegin );
    run::begin( batb.run.iterationRunEnd );

    // (now the non-core part of BATB is loaded by iterationRunBegin)

    
    batb.initialized_ = true;
}


// end BATB and its core parts
void end(BATB& batb)
{
    if ( batb.initialized_ )
    {
        // save the configuration to file
        batb.save();
        
        // (the non-core parts, created by iterationRunBegin, 
        // are unloaded by 'iterationRunEnd')

        run::end( batb.run.iterationRunEnd );
        run::end( batb.run.iterationRunBegin );

        gui::end( batb.gui );

        keys::end( batb.keys );

        value::end( batb.value );
        
        log::end( batb.log );
    }

    batb.initialized_ = false;


}


void BATB::save()
{
    // FIXME: write to file
}


}
