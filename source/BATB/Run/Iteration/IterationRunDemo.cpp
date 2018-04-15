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
#include "BATB/Run/Iteration/IterationRunDemo.hpp"
#include "BATB/Demo/Iteration/IterationDemoForest.hpp"
#include "OgreCamera.h"

namespace batb
{

namespace run
{


IterationRunDemo::IterationRunDemo(BATB* b, demo::World* d) : IterationRun( b ), demo_( d )
{

}



void IterationRunDemo::iterate_begin(World& run)
{

    // setup substack
    stack_ = { game::begin_iteration( new demo::IterationDemoForest( batb ) ) };

}


IterationStack IterationRunDemo::iterate_run(World& run)
{
    // wrap IterationDemo's inside IterationRun
    game::iterate( stack_, *demo_ );

    if ( stack_.empty() )
    {
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

