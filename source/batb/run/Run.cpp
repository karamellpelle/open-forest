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
#include "batb/event/EventList.hpp"
#include "batb/run/Run.hpp"
#include "batb/run/console/Console.hpp"
#include "batb/run/notify/Notifier.hpp"
#include "batb/run/KeySet.hpp"
#include "batb/run/iteration/IterationRunMain.hpp"
#include "batb/run/iteration/IterationRunOld.hpp"



namespace batb
{



namespace run
{

////////////////////////////////////////////////////////////////////////////////
//  Run

Run::Run(BATB* b) : ModuleBATB( b )
{
    console          = std::make_unique<Console>( b );
    notifier         = std::make_unique<Notifier>( b );
    keyset           = std::make_unique<KeySet>( b );
    iterationRunOld  = std::make_unique<IterationRunOld>( b );
    iterationRunMain = std::make_unique<IterationRunMain>( b );
}

Run::~Run()
{

}


////////////////////////////////////////////////////////////////////////////////
// 
// begin the non-core part of Run
void Run::begin(const std::string& path)
{

    BATB_LOG_FUNC( batb );

    if ( init_empty() )
    {
        // set config
        config( path );
        
        // TODO: use 'yaml' for configuration

        // load associated keys 
        keyset->load("batb/run/KeySet.yaml");
        // console key should not be disabled
        keyset->console->canDisable( false );

        // setup Console
        console->begin();

        // setup Notify
        notifier->begin(); 

        // begin non-core iterations:
        iterationRunOld->begin();
        iterationRunMain->begin();
    }

    init( true );
}


// end the non-core part of Run
void Run::end()
{
    BATB_LOG_FUNC( batb );

    if ( init_nonempty() )
    {
        save();


        // end non-core iterations:
        iterationRunMain->end();
        iterationRunOld->end();

        // end Notify
        notifier->end();

        // end Console
        console->end();

    }
    
    init( false );
}




} // namespace run

} // namespace batb
