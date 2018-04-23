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
#include "BATB/Run/Iteration/IterationRunDemo.hpp"
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
    // stack_ is set up previously.  that means iterations of 
    // demo::World will continue where it was the last time.
    // but we have to restart the iteration at stack
    
    if ( !stack_.empty() )
    {
        stack_.front()->iterate_begin( *demo_ );
    }
}


IterationStack IterationRunDemo::iterate_run(World& run)
{
    // wrap IterationDemo's inside IterationRun
    game::iterate( stack_, *demo_ );

    // this Run-Iteration decides to go back or not 
    // (IterationDemoForest runs forever)
    // TODO: look at events, see if we shall exit or not 
    if ( batb->run->keys->escape->click() )
    {
        // set back cursor
        batb->keys->setCursorFree( false );

        return _emptylist_;
    }
    else
    {
        return { this };
    }


}
////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb

