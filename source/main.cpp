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
#include "env.hpp"
#include "game.hpp"
#include "file.hpp"
#include "batb.hpp"


// use commandline to modify our BATB object
void cmdline(int argc, char** argv, BATB& batb)
{
    return 0;
}



int main(int argc, char** argv)
{
    int ret = 0;
 
    // our BATB object
    batb::BATB batb( file::dynamic_data( "batb.xml" ) );

    try
    {
        // create our environment
        env::begin( "env.xml" );

        // initialize the core parts of BATB.
        // the non-core part is created by 'iterationRunBegin'
        batb::begin( batb );

        // modify BATB from command line
        commandline( argc, argv, batb );

        batb::run::World run;
        batb::run::IterationStack stack;

        stack.next(
            batb.run.iterationRunBegin,    // create game data and continue with iterationRunMain (if success)
            batb.run.iterationRunEnd       // destroy game data
        );

        // "main loop"
        while ( !stack.empty() )
        {
            // begin frame for iteration
            env::frame_begin();

            // make 1 iteration of world
            stack.iterate( run ); 

            // end frame for iteration (swap buffers, poll events)
            env::frame_end();
        }
        
    }
    catch (std::exception& e)
    {
        // some serious error occured above, lets handle it
        std::cerr << THIS_FUNCTION << ": fatal error: " << e.what() << std::endl;
        ret = 1;
    }

    // shut down...

    batb::end( batb );

    env::end();

    return ret;
}

