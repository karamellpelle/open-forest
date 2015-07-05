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


IterationRunBegin::IterationRunBegin(BATB& b) : IterationRun( b ), 
                                                loader_( b )
{

}


void IterationRunBegin::iterate_begin(World& world)
{
    BATB_LOG_FUNC( batb );

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

    // load non-core on current context, but in background thread:
    loader_.begin();

}


void IterationRunBegin::iterate_run(IterationStack& stack, World& world)
{

    //GLFWwindow * window = glfwGetCurrentContext();
    //std::cout << __PRETTY_FUNCTION__ << "   ";
    //std::cout << "thread::id: " <<  std::this_thread::get_id();
    //std::cout << ", GLFWindow: " << window << std::endl;
    //
    //++iteration_count_;


    static FiniteLoad* current = nullptr;

    if ( FiniteLoad* cur = loader_.current() )
    {
        if ( cur != current ) 
        {
            uint percent = (uint)( 100.0 * cur->to_alpha() );
            const char* tag = cur->tag.c_str();
            std::printf( "[%3u%%] Now loading: %s\n", percent, tag );
        }

        current = cur;

        stack.next( this );
    }
    else
    {
        loader_.end();

        // we now succeded to load all parts of BATB (core and non-core),
        // so start the actual game in iterationRunMain
        stack.next( game::begin_iteration( batb.run.iterationRunMain ) );

        //stack.finish();
    }
    
    
}


void IterationRunBegin::Loader::run()
{
    //GLFWwindow * window = glfwGetCurrentContext();
    //std::cout << __PRETTY_FUNCTION__ << "   ";
    //std::cout << "thread::id: " <<  std::this_thread::get_id();
    //std::cout << ", GLFWindow: " << window << std::endl;

    // NOTE: number must be updated to correct number of loads
    FiniteLoad loading( 4 + 3 );

    
    try
    {

        // load gOGRE
        push_current( loading( "OGRE" ) );
        ogre::begin( batb.ogre );
        ++loading;

        // load AL
        push_current( loading( "AL" ) );
        al::begin( batb.al );
        ++loading;

        // load forest
        push_current( loading( "Forest" ) );
        forest::begin( batb.forest );
        ++loading;

        // load race
        //push_current( loading( "" ) );
        //race::begin( batb.race );
        //++loading;

        // load the non-core part of run
        push_current( loading( "Run" ) );
        run::begin( batb.run );
        ++loading;

        // tmp: fake loading, to show capabilities:
        push_current( loading( "Proxy library A" ) );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        ++loading;
        push_current( loading( "Proxy library B" ) );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        ++loading;
        push_current( loading( "Proxy library C" ) );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        ++loading;
    }
    catch (std::exception& e)
    {
        batb.log << "IterationRunBegin::Loader: error: " << e.what() << std::endl; 
        // TODO: handle exception into IterationRunBegin
    }

    // must be done to signalize completion
    push_current( nullptr );
}

////////////////////////////////////////////////////////////////////////////////
//

void begin(IterationRunBegin& iter)
{
    BATB_LOG_FUNC( iter.batb );
}

void end(IterationRunBegin& iter)
{
    BATB_LOG_FUNC( iter.batb );
}


} // namespace run

} // namespace batb

