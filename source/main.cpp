//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
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
#include "game.hpp"
#include "BATB.hpp"
#include "BATB/Run.hpp"
#include "BATB/Run/World.hpp"
#include "BATB/Run/Iteration.hpp"


// should we load the non-core part of batb on main thread?
// the other alternative is to use "workers"
#define LOAD_NONCOREBATB_ON_MAIN_THREAD



int main(int argc, char** argv)
{
    try
    {
        using namespace batb;

        // TODO: use YAML::LoadFile, modify node from cmdline, pass to module.
        //       if so, configurations from command line options will be saved
        //       to file.

        ////////////////////////////////////////////////////////////////////////////////
        // create and initiaze the core parts of our game environment
        // the non-core part is created by 'iterationRunBegin'
        //
        auto batb = std::make_unique<BATB>();
        batb->begin( file::dynamic_data( "BATB.yaml" ) ); 

        // this is the application world
        run::World run;

        // who is playing this game?
        run.player = batb->player();

#ifdef LOAD_NONCOREBATB_ON_MAIN_THREAD
        ////////////////////////////////////////////////////////////////////////////////
        // load resources in main thread
        // this does not segfault during shutdown, but on the other side,
        // no unloading of batb is done :)
        batb->beginNonCore();
        run::IterationStack stack =
        {
              game::begin_iteration( batb->run->iterationRunMain ),     // main
        };
#else 
        ////////////////////////////////////////////////////////////////////////////////
        // load non-core batb on dedicated thread.
        // this causes segfault during shutdown. not sure why, probably Ogre3D
        //
        auto* loadBATB = new run::IterationRunWork( batb, run::LoadWorker<BATB>( batb ) );
        auto* unloadBATB = new run::IterationRunWork( batb, run::UnloadWorker<BATB>( batb ) );
        run::IterationStack stack =
        {
              game::begin_iteration( loadBATB ),                      // create the non-core part of BATB
              game::begin_iteration( batb->run->iterationRunMain ),     // main
              game::begin_iteration( unloadBATB )                     // destroy game data at end
        };
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // "main loop"

        while ( !stack.empty() )
        {
            // begin frame for iteration
            batb->frameBegin();

            // make one iteration of the application world
            iterate( stack, run );

            // end frame for iteration
            batb->frameEnd();
        }

        batb->log << "IterationStack empty. " << batb->screen->frameCount() << " screen frames rendered.\n";

        ////////////////////////////////////////////////////////////////////////////////
        
#ifndef LOAD_NONCOREBATB_ON_MAIN_THREAD
        batb->end();
#endif

    }
    catch (std::exception& e)
    {
        ////////////////////////////////////////////////////////////////////////////////
        // some serious error occured above, lets handle it
        //
        std::cerr << "OpenForest ERROR: " << e.what() << std::endl;

        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // successful run

    std::cout << "OpenForest exited normally. Peace & Love" << std::endl;
    return 0;

}
