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
void commandline(int argc, char** argv, batb::BATB& batb)
{

}


void testXML()
{
    using namespace batb;

    xml::Document xml; std::string errstr;
    if ( auto err = xml::load_document( xml, "data/tmp.xml", THIS_FUNCTION, errstr ) )
    {
        std::cout << "could not test XML: " << errstr << std::endl;
        return;
    }

    // TODO: these expansions does not work. instead return an object with 
    //       variadic template operator()!
    std::string str;
    batb::uint n {};
    bool b {};
    std::cout << "A: " << std::endl;
    if ( xml::read( xml, "boat", "apple", str ) )
    {
        std::cout << "str == " << str << std::endl;
    }
    else std::cout << "false." << std::endl;
    std::cout << std::endl;
    std::cout << "B: " << std::endl;
    if ( xml::read( xml, "boat", "apple", str, n ) )
    {
        std::cout << "str == " << str << ", n == " << n << std::endl;
    }
    else std::cout << "false." << std::endl;
    std::cout << std::endl;
    std::string boat = "boat";
    std::cout << "C: " << std::endl;
    if ( xml::read( xml, boat, "apple", str, n ) )
    {
        std::cout << "str == " << str << ", n == " << n << std::endl;
    }
    else std::cout << "false." << std::endl;
    std::cout << std::endl;
    std::string apple = "apple";
    std::cout << "D: " << std::endl;
    if ( xml::read( xml, "boat", apple, str, n ) )
    {
        std::cout << "str ==" << str << ", n == " << n << std::endl;
    }
    else std::cout << "false." << std::endl;
    std::cout << std::endl;
    std::cout << "E: " << std::endl;
    if ( xml::read( xml, "boat", "apple" ) )
    {
        std::cout << "true." << std::endl;
    }
    else std::cout << "false." << std::endl;
    std::cout << std::endl;

    
}

int main(int argc, char** argv)
{
    int ret = 0;
 
    // our BATB object
    batb::BATB batb( file::dynamic_data( "batb/BATB.xml" ) );

    try
    {
        // create our environment
        env::begin( file::dynamic_data( "env.xml" ) );

        // initialize the core parts of BATB.
        // the non-core part is created by 'iterationRunBegin'
        batb::begin( batb );

        // modify BATB from command line
        commandline( argc, argv, batb );

        // main is iterating batb::run::World
        batb::run::World run;
        batb::run::IterationStack stack;

        stack.next(
              game::begin_iteration( batb.run.iterationRunBegin ),  // create the non-core part of BATB, continue with iterationRunMain, if success
              game::begin_iteration( batb.run.iterationRunEnd )     // destroy game data
        );
testXML();
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
        std::cerr << THIS_FUNCTION << ": fatal error: \n" << e.what() << std::endl;
        ret = 1;
    }

    // shut down...

    env::end();

    batb::end( batb );


    return ret;
}

