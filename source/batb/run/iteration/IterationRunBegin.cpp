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
#include "batb.hpp"

namespace batb
{

namespace run
{


IterationRunBegin::IterationRunBegin(BATB& b) : IterationRun( b )
{

}


void IterationRunBegin::iterate_begin(World& world)
{
    batb.log << THIS_FUNCTION << std::endl;

    // NOTE:
    // this function should ideally load the non-core part of BATB in a seperate thread,
    // and wait in 'iterate' until completion, showing interactive output in the mean time.
    // for example using std::async. however, there is a real problem with IO on different
    // threads. especially OpenGL: we need OpenGL to show the progress, and at the same
    // time use OpenGL in the loading process in the other thread. we could use mutex locks
    // to gain exclusive access for IO on threads, but this will not work very well. 
    // (and mutex locks are not allowed at all for std::async...). the solution will be to
    // have the invariant that this thread only touches OpenGL during IO, and the loading
    // thread use IO fully, but use a different OpenGL context. also, these two threads
    // must work on disjoint memory as usual, but this should not be a problem, since
    // this thread is not interrested in BATB/run::World.
    //
    // for now, just present a "loading..." frame, and load the non-core part of BATB
    // on the same thread, in a blocking manner.
}


void IterationRunBegin::iterate_run(IterationStack& stack, World& world)
{
    batb.log << THIS_FUNCTION << std::endl;

    if ( iteration_count_ == 0 )
    {
        // output "loading...", do actual loading on next iteration
        batb.log << "now loading..." << std::endl;

        stack.next( this );
    }
    else
    {
        // load & block
        if ( begin_non_core() )
        {
            stack.next( game::begin_iteration( batb.run.iterationRunMain ) );
        }
        else
        {
            stack.finish(); 
        }
    }

    ++iteration_count_;
}


// load the non core part
bool IterationRunBegin::begin_non_core()
{
    batb.log << "loading non-core part of BATB... ";

    try
    {
        // load forest
        //forest::begin( batb.forest );

        // load race
        //race::begin( batb.race );

        // load the rest of run
        run::begin( batb.run );
    }
    catch (std::exception& e)
    {
        batb.log << "failure: " << e.what() << std::endl; 
        return false;
    }

    // loading complete.
    batb.log << "OK" << std::endl;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//

void begin(IterationRunBegin& iter)
{
    iter.batb.log << THIS_FUNCTION << std::endl;
}

void end(IterationRunBegin& iter)
{
    iter.batb.log << THIS_FUNCTION << std::endl;
}


} // namespace run

} // namespace batb

