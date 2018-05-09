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
#include "BATB/Run.hpp"
#include "BATB/Run/KeySet.hpp"
#include "BATB/Run/Notify.hpp"
#include "BATB/Run/Iteration/IterationRunDemo.hpp"
#include "BATB/Run/events.hpp"
#include "BATB/Demo.hpp"
#include "BATB/Demo/Iteration/IterationDemoForest.hpp"
#include "OgreCamera.h"

namespace batb
{

namespace run
{


IterationRunDemo::IterationRunDemo(BATB* b) : IterationRun( b )
{

}

void IterationRunDemo::begin()
{

    // setup substack, that is, stack of IterationDemo's
    // (IterationDemoForest runs forever)
    stack_ = { game::begin_iteration( new demo::IterationDemoForest( batb ) ) };
}

void IterationRunDemo::demoWorld(demo::World* demo)
{
    demo_ = demo;
}

void IterationRunDemo::iterate_begin(World& run)
{
    // IterationStack for demo::World was previously set up.  that means 
    // iterations of demo::World will continue where it was the last time,
    // but we have to restart (i.e. 'iterate_begin') the previous iteration
    
    if ( !stack_.empty() )
    {
        // notify message
        std::ostringstream msg; msg << "use " << batb->run->keys->escape->nameGUI() << " to go back";
        batb->run->notify->message( msg.str(), 4.0 );

        stack_.front()->iterate_begin( *demo_ );
    }
}


IterationStack IterationRunDemo::iterate_run(World& run)
{
    // wrap IterationDemo's inside IterationRun
    game::iterate( stack_, *demo_ );

    // this Run-Iteration decides to go back or not 
    // (IterationDemoForest runs forever)

    
    ////////////////////////////////////////////////////////////////
    // do
    if ( batb->run->keys->escape->click() )  run.events.push( event::Do::PopIteration );



    ////////////////////////////////////////////////////////////////
    // think

    // think: look at events and handle thereafter
    for ( auto& event : run.events )
    {
        if ( auto* next = eat<event::Do>( event ) )
        {
            switch ( *next )
            {
            case event::Do::Exit:
            {
                // set back cursor
                batb->keys->setCursorFree( false );

                // keep this event, making IterationRunMain exit too
                event->keepAlive();
                
                // finish this iteration, IterationRunMain
                return _emptylist_;
            }
            case event::Do::PopIteration:
            {
                // set back cursor
                batb->keys->setCursorFree( false );

                // finish this iteration, IterationRunMain
                return _emptylist_;
            }
            }
        }
    }

    // continue
    return { this };

}
////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb

