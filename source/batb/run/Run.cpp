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
#include "batb.hpp"




namespace batb
{



namespace run
{


////////////////////////////////////////////////////////////////////////////////
//  Run

Run::Run(BATB& b) : batb( b ), keyset( b ), 
                    iterationRunBegin( b ), 
                    iterationRunEnd( b ),
                    iterationRunMain( b ),
                    iterationRunOld( b )
{

}



void Run::save()
{

    // FIXME: write to file
}


////////////////////////////////////////////////////////////////////////////////
// 
// begin the non-core part of Run
void begin(Run& run)
{

    BATB_LOG_FUNC( run.batb );


    // set up this Run object from file
    YAML::Node yaml = YAML::LoadFile( run.filepath_ );

    // load associated keys 
    run.keyset.load("batb/run/KeySet.yaml");

    // begin non-core iterations:
    run::begin( run.iterationRunOld );
    run::begin( run.iterationRunMain );

    // set up GUI's
    run.guiMain = new GUIMain( run.batb );

    run.initialized_ = true;
}

// end the non-core part of Run
void end(Run& run)
{
    BATB_LOG_FUNC( run.batb );

    if ( run.initialized_ )
    {
        run.guiMain = nullptr; // GUI memory handled by TB

        // end non-core iterations:
        run::end( run.iterationRunMain );
        run::end( run.iterationRunOld );

        // clear keys
        run.keyset.clear();

        run.save();
    }
    
    run.initialized_ = false;

}


} // namespace run

} // namespace batb
