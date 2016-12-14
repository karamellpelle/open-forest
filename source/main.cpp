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
#include "include.hpp"
#include "env.hpp"
#include "game.hpp"
#include "file.hpp"
#include "batb.hpp"
#include "batb/run/workers.hpp" 
#include "batb/demo/other.hpp"



//void commandline_env(int argc, char** argv, YAML::Node& yaml)
//{
//
//}
//void commandline_batb(int argc, char** argv, YAML::Node& yaml)
//{
//
//}


int main(int argc, char** argv)
{
    int ret = 0;

    // TODO: use YAML::LoadFile, modify node from cmdline, pass to module.
    //       if so, configurations from command line options will be saved
    //       to file.

    env::Env env;
    env.config( file::dynamic_data( "env/Env.yaml" ) );

    // our BATB object
    batb::BATB batb( env );
    batb.config( file::dynamic_data( "batb/BATB.yaml" ) );


    try
    {
            
        // create our environment
        env::begin( env );

        // initialize the core parts of BATB.
        // (the non-core part is created by 'iterationRunBegin')
        batb::begin( batb );

        batb::demo::other_begin( batb ); // FIXME: remove

        using namespace batb;
        
        // application world
        run::World run;
        run.player = batb::run::local_player();
        

        auto* loadBATB = new run::IterationRunWork( batb, run::LoadWorker<BATB>( batb ) );
        auto* unloadBATB = new run::IterationRunWork( batb, run::UnloadWorker<BATB>( batb ) );
        run::IterationStack stack =
        {
              game::begin_iteration( loadBATB ),                      // create the non-core part of BATB
              game::begin_iteration( batb.run.iterationRunMain ),     // main
              game::begin_iteration( unloadBATB )                     // destroy game data at end
        };

  
  
        // "main loop"
        while ( !stack.empty() )
        {
            // begin frame for iteration
            env.frameBegin();

            // make 1 iteration of 'run'
            iterate( stack, run );

            // end frame for iteration
            env.frameEnd();
        }

        batb::demo::other_end( batb ); // FIXME: remove
    }
    catch (std::exception& e)
    {
        // some serious error occured above, lets handle it
        std::cerr << "main: " << e.what() << std::endl;
        ret = 1;
    }


    batb::end( batb );

    env::end( env );

    return ret;
}
