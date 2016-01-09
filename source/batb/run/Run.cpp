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
#include "batb/run/Console.hpp"



namespace batb
{



namespace run
{


////////////////////////////////////////////////////////////////////////////////
//  Run

Run::Run(BATB& b) : ModuleBATB( b ), console( b ), keyset( b ), 
                    iterationRunMain( b ),
                    iterationRunOld( b )
{

}



////////////////////////////////////////////////////////////////////////////////
// 
// begin the non-core part of Run
void begin(Run& run)
{

    BATB_LOG_FUNC( run.batb );


    if ( run.init_empty() )
    {
        // load associated keys 
        run.keyset.load("batb/run/KeySet.yaml");
        // console key should not be disabled
        run.keyset.console->canDisable( false );

        // setup Console
        run::begin( run.console );

        // begin non-core iterations:
        run::begin( run.iterationRunOld );
        run::begin( run.iterationRunMain );
    }

    run.init( true );
}

// end the non-core part of Run
void end(Run& run)
{
    BATB_LOG_FUNC( run.batb );

    if ( run.init_nonempty() )
    {
        run.save();


        // end non-core iterations:
        run::end( run.iterationRunMain );
        run::end( run.iterationRunOld );

        // end Console
        run::end( run.console );

    }
    
    run.init( false );
}


} // namespace run

} // namespace batb
