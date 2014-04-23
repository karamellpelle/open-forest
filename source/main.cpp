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
#include "Game.hpp"
#include "Env.hpp"
#include "BATB.hpp"



int main(int argc, char** argv)
{
    
    // init Env
    // TODO: init from settings (settings from file?)
    Env::begin();

    // set up environment (GL, ...)
    // ...


    using namespace BATB;

    // init BATB,
    // i.e. create resourceRunData. ForestData created by 'iterationDataBegin'...
    BATB::begin();

    RunWorld run;
    Game::IterationStack<RunWorld> stack;

    stack.push( resourceRunData()->prim->iterationDataBegin,    // create game data
                resourceRunData()->prim->iterationIntro,        // game intro
                resourceRunData()->prim->iterationMain,         // "main menu"
                resourceRunData()->prim->iterationOutro,        // game outro
                resourceRunData()->prim->iterationDataEnd       // destroy game data
              );

    BATB::log << "Game::IterationStack<RunWorld> starting. " << std::endl;

    // "main loop"
    while ( !stack.empty() )
    {
        // make 1 iteration of RunWorld:
        stack.iterate( run ); 
    }

    BATB::log << "Game::IterationStack<RunWorld> empty. " << std::endl;

    // end BATB
    BATB::end();

    Env::end();
}
