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
#include "glm/gtx/euler_angles.hpp"


namespace batb
{

namespace forest
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationForestDemo::IterationForestDemo(BATB& b) : IterationForest( b )
{

}



void IterationForestDemo::iterate_begin(World& forest)
{
    BATB_LOG_FUNC( batb );

    // we want clean state for our Key's, no garbage:
    batb.forest.keyset.reset();

    if ( !forest.runners.empty() )
    {
        aiming = &forest.runners.front(); // is this pointer safe??
    }
    std::cout << "Starting Forest demo. " << std::endl;
}


void IterationForestDemo::iterate_forest(IterationStack& stack, World& forest)
{


    //if ( batb.forest.keyset.u->click() ) forest.toggle_a = !forest.toggle_a;
    //if ( batb.forest.keyset.i->click() ) {forest.toggle_b = !forest.toggle_b; std::cout << "FPS: " << env::frame_fps() << std::endl; }
    //if ( batb.forest.keyset.ogre->click() ) forest.toggle_ogre = !forest.toggle_ogre;
    //if ( batb.forest.keyset.nanovg->click() ) forest.toggle_nanovg = !forest.toggle_nanovg;
    //if ( batb.forest.keyset.tb->click() ) forest.toggle_tb = !forest.toggle_tb;


    ////////////////////////////////////////////////////////////////////////////////
    //  STEP
    // 

    ////////////////////////////////////////
    // * set aim_x_ based on input and run-time
    // a_acc, b_acc = input
    double x, y;
    glfwGetCursorPos( env::screen_window(), &x, &y );
    std::cout << "\rcursor: " << x << " " << y << std::flush;

    constexpr tick_t aim_dt = 0.02;

    tick_t aim_tick_next = forest.run.tick;
    while ( aim_tick_next + aim_dt <= aim_tick_next )
    {
        // TODO: step aim_ab based on input 
        aim_a += 0.0;
        aim_b += 0.0;

        aim_tick += aim_dt;
    }

    ////////////////////////////////////////
    // set aim of runner from aim_x_
    if ( aiming )
    {
        aiming->aim = glm::eulerAngleYXZ( aim_a, aim_b, aim_c );
    }


    ////////////////////////////////////////
    // * step dt of runner
    constexpr tick_t dt = 0.02;

    tick_t tick_next = forest.run.tick;
    while ( forest.tick + dt <= tick_next )
    {
       
        // TODO: step 'aiming' (physics)
        //
        forest.tick += dt;
    }



    // continue with this itertion, unless stack handled
    return stack.next( this );

}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationForestDemo& iter)
{
    BATB_LOG_FUNC( iter.batb );


}

void end(IterationForestDemo& iter)
{
    BATB_LOG_FUNC( iter.batb );
}



} // namespace forest

} // namespace batb


