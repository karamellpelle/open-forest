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
#include "batb.hpp"
#include "batb/run/workers.hpp"
#include "batb/run/Run.hpp"
#include "batb/value/run.hpp"
#include "batb/keys/Keys.hpp"
#include "batb/ogre/OGRE.hpp"
#include "batb/al/AL.hpp"
#include "batb/gl/GL.hpp"
#include "batb/gui/GUI.hpp"
#include "batb/forest/Forest.hpp"
#include "batb/demo/Demo.hpp"


#define LOAD_PROXY

  
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

        // load OGRE
        ////////////////////////////////////////////////////////////////////////////////
        // NOTE: when loading Ogre in different GL context, the Terrain component becomes
        //       diffused and cause some minor rendering artifacts when changing back
        //       to main context. 
        // load Ogre
        work.state( "OGRE" );
        batb->ogre->begin( file::directory( batb->filepath() ) + "/ogre/OGRE.yaml" );

        // load the non-core part of Run
        work.state( "Run" );
        batb->run->begin( file::directory( batb->filepath() ) + "/run/Run.yaml" );

        // load Forest
        work.state( "Forest" );
        batb->forest->begin( file::directory( batb->filepath() ) + "/forest/Forest.yaml" );

        // load demo
        work.state( "Demo" );
        batb->demo->begin();


#ifdef LOAD_PROXY
        // tmp: fake loading, to show capabilities:
        work.state( "Proxy library A" );
        std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
        work.state( "Proxy library B" );
        std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
        work.state( "Proxy library C" );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
#endif

    }
    catch (std::exception& e)
    {
        batb->log << "error loading : " << e.what() << std::endl; 
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
        batb->demo->end();

        // unload race
        //race::begin( batb.race );

        // unload forest
        batb->forest->end();

        // unload the non-core part of run
        batb->run->end();


        // unload OGRE
        // FIXME!!! current thread is not main. 
        //          OGRE::frameBegin is calledafter delete!
        work.state( "OGRE" );
        batb->ogre->end();

        //// unload AL
        //work.state( "AL" );
        //al::end( batb.al );


#ifdef LOAD_PROXY
        work.state( "Proxy library B" );
        std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
        work.state( "Proxy library A" );
        std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
#endif

    } 
    catch (std::exception& e)
    {
        batb->log << "error unloading : " << e.what() << std::endl; 
    }

    // must be done to signalize completion
    work.finish();
}


////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb

