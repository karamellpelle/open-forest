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
#include "batb/run/workers.hpp"


//#define LOAD_PROXY

  
namespace batb
{

namespace run
{



void LoadWorker<BATB>::operator()(Work& work)
{
    // (this is done in other GL context!)

    // setup context to our invariant
    gl::init_state();

    // how many steps to be loaded
#ifdef LOAD_PROXY
    work.definite( 4 + 3 ); 
#else
    work.definite( 4 ); 
#endif

    try
    {

        // load gOGRE
        ////////////////////////////////////////////////////////////////////////////////
        // NOTE: when loading Ogre in different GL context, the Terrain component becomes
        //       diffused and cause some minor rendering artifacts when changing back
        //       to main context. 
        work.state( "OGRE" );
        ogre::begin( batb.ogre );

        // load AL
        work.state( "AL" );
        al::begin( batb.al );

        // load the non-core part of Run
        work.state( "Run" );
        run::begin( batb.run );

        // load Forest
        work.state( "Forest" );
        forest::begin( batb.forest );

        // load demo
        work.state( "Demo" );
        demo::begin( batb.demo );

#ifdef LOAD_PROXY
        // tmp: fake loading, to show capabilities:
        work.state( "Proxy library A" );
        std::this_thread::sleep_for( std::chrono::seconds( 8 ) );
        work.state( "Proxy library B" );
        std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
        work.state( "Proxy library C" );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
#endif

    }
    catch (std::exception& e)
    {
        batb.log << "error loading : " << e.what() << std::endl; 
    }

    // must be done to signalize completion
    work.finish();
}


void UnloadWorker<BATB>::operator()(Work& work)
{
    // (this is done in other GL context!)

    // unknown how many steps to be taken
    work.indefinite(); 

    try
    {
        // unload Demo
        demo::end( batb.demo );

        // unload race
        //race::begin( batb.race );

        // unload forest
        forest::end( batb.forest );

        // unload the non-core part of run
        run::end( batb.run );


        // unload AL
        work.state( "AL" );
        al::end( batb.al );

        // unload OGRE
        // FIXME!!! current thread is not main. 
        //          OGRE::frameBegin is calledafter delete!
        work.state( "OGRE" );
        ogre::end( batb.ogre );

#ifdef LOAD_PROXY
        work.state( "Proxy library B" );
        std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
        work.state( "Proxy library A" );
        std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
#endif

    } 
    catch (std::exception& e)
    {
        batb.log << "error unloading : " << e.what() << std::endl; 
    }

    // must be done to signalize completion
    work.finish();
}


////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb

