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
#include "Env.hpp"
#include "BATB.hpp"
#include "Game.hpp"



// control Env from commandline, by modifying Config object
Env::Config* cmdline_env(int argc, char** argv, Env::Config* init)
{
    return init;
}


// control BATB from commandline, by modifying BATB::Config object
BATB::Config* cmdline_batb(int argc, char** argv, BATB::Config* cfg)
{
    return cfg;
}




int main(int argc, char** argv)
{
    try
    {
        // init Env
        Env::Config env_config( File::dynamicData( "env.xml" ) );
        Env::begin( cmdline_env( argc, argv, &env_config ) );

        // FIXME: now set up
        //        - GL invariants (defined in readme/)
        //        - AL invariants
        //        - ...


        using namespace BATB;


        // init BATB.
        // this creates only the necessary part of resourceRunData, and the rest
        // is created by 'iterationRunDataBegin'
        BATB::Config batb_cfg( File::dynamicData( "batb.xml" ) );
        BATB::begin( cmdline_batb( argc, argv, &batb_cfg ) );

        RunWorld run;
        Game::IterationStack<RunWorld> stack;

        stack.push( resourceRunData()->prim->iterationRunBegin,    // create game data, continue with iterationRunMain (if success)
                    resourceRunData()->prim->iterationRunEnd       // destroy game data
                  );

        BATB::log << "Game::IterationStack<RunWorld> starting. " << std::endl;

        // "main loop"
        while ( !stack.empty() )
        {
            // begin frame for iteration
            Env::frameBegin();

            // make 1 iteration of RunWorld:
            stack.iterate( run ); 

            // end frame for iteration (swap buffers, poll events)
            Env::frameEnd();
        }

        BATB::log << "Game::IterationStack<RunWorld> empty. " << std::endl;

    }
    catch (std::exception& e)
    {
        std::cerr << "main:    FATAL: " << e.what() << std::endl;
    }

    // end BATB
    BATB::end();

    // end Env
    Env::end();
}
