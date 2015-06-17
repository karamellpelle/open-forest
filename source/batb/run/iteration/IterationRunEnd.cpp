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

namespace batb
{

namespace run
{


IterationRunEnd::IterationRunEnd(BATB& b) : IterationRun( b )
{

}


void IterationRunEnd::iterate_begin(World& world)
{
    BATB_LOG_FUNC( batb );

    // unload the non-core part created by IterationRunBegin.
    end_non_core();
}

void IterationRunEnd::iterate_run(IterationStack& stack, World& world)
{
    BATB_LOG_FUNC( batb );

}

void IterationRunEnd::end_non_core()
{


    // unload the non-core part of run
    run::end( batb.run );

    // unload race
    //race::begin( batb.race );

    // unload forest
    //forest::begin( batb.forest );

    // unload AL
    al::end( batb.al );

    // unload OGRE
    ogre::end( batb.ogre );
}

////////////////////////////////////////////////////////////////////////////////
//

void begin(IterationRunEnd& iter)
{
    BATB_LOG_FUNC( iter.batb );
}

void end(IterationRunEnd& iter)
{
    BATB_LOG_FUNC( iter.batb );
}


} // namespace run

} // namespace batb

