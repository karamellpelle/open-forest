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
#include "BATB.hpp"
#include "BATB/Player.hpp"
#include "BATB/Value.hpp"
#include "BATB/Screen.hpp"
#include "BATB/Time.hpp"
#include "BATB/Keys.hpp"
#include "BATB/GL.hpp"
#include "BATB/GUI.hpp"
#include "BATB/AL.hpp"
#include "BATB/OGRE.hpp"
#include "BATB/Run.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Demo.hpp"

namespace batb
{



BATB::BATB() 
{  

    log    = std::make_unique<Log>();
    value  = std::make_unique<value::Value>( this );
    screen = std::make_unique<screen::Screen>( this );
    time   = std::make_unique<time::Time>( this );
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
// it tries to delete pointers to unknown types in the header file
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


        ////////////////////////////////////////////////////////////////////////////////
        // begin core
        //log->begin();
        value->begin(   file::directory( filepath() ) + "/BATB/Value.yaml" );
        screen->begin(  file::directory( filepath() ) + "/BATB/Screen.yaml" );
        time->begin(    file::directory( filepath() ) + "/BATB/Time.yaml" );
        keys->begin(    file::directory( filepath() ) + "/BATB/Keys.yaml" );
        gui->begin(     file::directory( filepath() ) + "/BATB/GUI.yaml" );
        gl->begin(      file::directory( filepath() ) + "/BATB/GL.yaml" );
        al->begin(      file::directory( filepath() ) + "/BATB/AL.yaml" );

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

        // (the non-core part of BATB should be unloaded before this)

        ////////////////////////////////////////////////////////////////////////////////
        // end core part
        al->end();
        gui->end();
        gl->end();
        keys->end();
        time->end();
        screen->end();
        value->end();

    }

    init( false );

}

// load non-core batb in current thread
void BATB::beginNonCore()
{
    // load Ogre
    ogre->begin( file::directory( filepath() ) + "/BATB/OGRE.yaml" );

    // load the non-core part of Run
    run->begin( file::directory( filepath() ) + "/BATB/Run.yaml" );

    // load Forest
    forest->begin( file::directory( filepath() ) + "/BATB/Forest.yaml" );

    // load demo
    demo->begin();

}



void BATB::frameBegin()
{
    screen->frameBegin();
    time->frameBegin();
    // ...
}

void BATB::frameEnd()
{
    // ...
    time->frameEnd();
    screen->frameEnd();
}

Player* BATB::player()
{
    // tmp:
    static Player ret;

#ifdef __unix__
    if ( auto pw = ::getpwuid( ::geteuid() ) )
    {
        ret.name = std::string( pw->pw_name );
    }
    else
#endif
    ret.name = "open-forest";

    return &ret;
}


}
