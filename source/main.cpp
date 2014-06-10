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
#include "Game.hpp"
#include "File.hpp"
#include "BATB.hpp"


// control BATB from commandline, by modifying BATB::Config object
int cmdline(int argc, char** argv, BATB::xml::XMLElement* xml)
{
    return 0;
}



int main(int argc, char** argv)
{
    int ret = 0;

    try
    {
        using namespace BATB::xml;

        // this is our program configuration
        BATB::Config batb_cfg( File::dynamicData( "batb.xml" ) );
        batb_cfg.Print(); // TMP
        XMLHandle xml( batb_cfg.FirstChildElement("BATB") );
       
        // modify from command line, take actions
        if ( int ret = cmdline( argc, argv, xml.ToElement() ) )
        {
            return ret;
        }

        // init Env, from configuration
        Env::begin( xml.FirstChildElement("Env").ToElement() );


        //////////////////////////////////////////////////////////
        //      OpenGL

        //////////////////////////////////////////////////////////
        //      OpenAL


        using namespace BATB;


        // init BATB.
        // this creates only the necessary part of Run, and the rest
        // is created by 'iterationRunBegin'
        BATB::begin( &batb_cfg );

        RunWorld run;
        Game::IterationStack<RunWorld> stack;

        stack.push( theRun()->prim->iterationRunBegin,    // create game data, continue with iterationRunMain (if success)
                    theRun()->prim->iterationRunEnd       // destroy game data
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
        ret = 1;
    }

    // end BATB
    BATB::end();

    // end Env
    Env::end();



    // quit...
    return ret;
}

