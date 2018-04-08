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
#include "batb/BATB.hpp"
#include "batb/value/Value.hpp"
#include "batb/keys/Keys.hpp"
#include "batb/gl/GL.hpp"
#include "batb/gui/GUI.hpp"
#include "batb/al/AL.hpp"
#include "batb/ogre/OGRE.hpp"
#include "batb/run/Run.hpp"
#include "batb/forest/Forest.hpp"
#include "batb/demo/Demo.hpp"
#include "env/Env.hpp"
#include <chrono>

namespace batb
{



BATB::BATB() 
{  

    log    = std::make_unique<Log>();
    env    = std::make_unique<env::Env>();
    value  = std::make_unique<value::Value>( this );
    keys   = std::make_unique<keys::Keys>( this );
    gl     = std::make_unique<gl::GL>( this );
    gui    = std::make_unique<gui::GUI>( this );
    al     = std::make_unique<al::AL>( this );
    ogre   = std::make_unique<ogre::OGRE>( this );
    run    = std::make_unique<run::Run>( this );
    forest = std::make_unique<forest::Forest>( this );
    demo   = std::make_unique<demo::Demo>( this );
    
}

// without this, the compiler complaints about missing class definitions because
// it tries to delete pointers to unknown types
BATB::~BATB()
{

}



// initialize BATB and its core parts
void BATB::begin(const std::string& path)
{
    if ( init_empty() )
    {
        // set configuration file
        config( path );


        // FIXME: remove Env!
        // create and initialize our environment
        env->config( file::dynamic_data( "env/Env.yaml" ) );
        env::begin( *env );

        ////////////////////////////////////////////////////////////////////////////////
        // begin core
        value->begin( file::directory( filepath() ) + "/value/Value.yaml" );
        keys->begin(  file::directory( filepath() ) + "/keys/Keys.yaml" );
        gui->begin(   file::directory( filepath() ) + "/gui/GUI.yaml" );
        gl->begin(    file::directory( filepath() ) + "/gl/GL.yaml" );
        al->begin(    file::directory( filepath() ) + "/al/AL.yaml" );

        // now configure module from 'yaml'
        // ...
        
        ////////////////////////////////////////////////////////////////////////////////
        // (non core initialized later)

    }

    init( true );
    
}


// end BATB and its core parts
void BATB::end()
{
    if ( init_nonempty() )
    {
        // save the configuration to file
        save();

        // (the non-core part of BATB is unloaded by IterationRunWork)

        ////////////////////////////////////////////////////////////////////////////////
        // end core part
        al->end();
        gui->end();
        gl->end();
        keys->end();
        value->end();

        // FIXME
        env::end( *env );
    }

    init( false );

}

// load non-core batb in current thread
void BATB::beginNonCore()
{
    // load Ogre
    ogre->begin( file::directory( filepath() ) + "/ogre/OGRE.yaml" );

    // load the non-core part of Run
    run->begin( file::directory( filepath() ) + "/run/Run.yaml" );

    // load Forest
    forest->begin( file::directory( filepath() ) + "/forest/Forest.yaml" );

    // load demo
    demo->begin();

}



void BATB::frameBegin()
{
    // tmp!
    env->frameBegin();
}

void BATB::frameEnd()
{
    // tmp!
    env->frameEnd();
}

}
