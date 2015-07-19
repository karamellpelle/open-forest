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
#include <chrono>

namespace batb
{



BATB::BATB() : log( *this ), value( *this ), keys( *this ), gui( *this ), ogre( *this ), al( *this ),
               run( *this ), forest( *this )
{

    
}


// initialize BATB and its core parts
void begin(BATB& batb)
{
    if ( batb.init_empty() )
    {
        // core:
        batb.value.config(     file::directory( batb.filepath ) + "/value/Value.yaml" );
        batb.gui.config(       file::directory( batb.filepath ) + "/gui/GUI.yaml" );

        // non-core:
        batb.ogre.config(      file::directory( batb.filepath ) + "/ogre/OGRE.yaml" );
        batb.al.config(        file::directory( batb.filepath ) + "/al/AL.yaml" );
        batb.run.config(       file::directory( batb.filepath ) + "/run/Run.yaml" );
        batb.forest.config(    file::directory( batb.filepath ) + "/forest/Forest.yaml" );


        // logging
        log::begin( batb.log );

        // general values to use 
        value::begin( batb.value );


        // now configure module from 'yaml'
        // ...
        


        //////////////////////////////////////////////////////////
        //      OpenGL
        // BATB assumes this GL state:
        gl::init_state();


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
    }

    batb.init( true );
    
}


// end BATB and its core parts
void end(BATB& batb)
{
    if ( batb.init_nonempty() )
    {
/*
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::chrono::system_clock::duration dtn = tp.time_since_epoch();

        std::ostringstream os;
        os << "save-tick is " << dtn.count();
        batb.yaml[ "TestSave" ] = os.str();
*/
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

    batb.init( false );

}



}
