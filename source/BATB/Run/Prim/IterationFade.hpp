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
#ifndef BATB_ITERATION_FADE_HPP
#define BATB_ITERATION_FADE_HPP
#include "BATB/Run/Prim/Iteration.hpp"

namespace BATB
{


// interactive fading IterationA -> IterationB. a nice effect. TODO.
class IterationFade : public IterationRunWorld
{
public:
    static IterationFade* create(IterationRunWorld* a, IterationRunWorld* b) { return new IterationFade( a, b ); }
    static void destroy(IterationFade* iter) { destroy_(); }

    void ticks(tick_t t) { ticks_ = t; }

    void iterate(IterationStackRunWorld& stack, RunWorld& run)
    {
        static IterationStackRunWorld proxy;

        // set tick_ on first iteration
        if ( tick_empty_ )
        {
            tick_ = Env::tick();
            tick_empty_ = false;
        }


        tick_t alpha = (Env::tick() - tick_) / ticks_;
        keep_inside( 0.0, 1.0, alpha );


        // framebuffers:
        // FBO0: main FBO (screen)
        // FBO1: auxilliary FBO (offscreen)


        // output of iterationB to auxilliary FBO1
        Env::screenFBOSet1();
        iterationB_->iterate( proxy, a );

        // TODO: render FBO1 to FBO0, opacity 1.0

        // output of iterationB to auxilliary FBO1
        Env::screenFBOSet1();
        iterationA_->iterate( proxy, a );

        // TODO: render FBO1 to FBO0, opacity 1.0 - alpha

        // do not fill up proxy
        proxy.clear();

        // think
        if ( 1.0 == alpha )
        {
            stack.push();
        }
        else
        {
            stack.push( this );
        }


    }

private:
    IterationFade(IterationRunWorld* a, IterationRunWorld* b) : iterationA_( a ), iterationB_( b ), tick_empty_( true )
    {
    }
    void destroy_() { delete this; }

    IterationRunWorld* iterationA_;
    IterationRunWorld* iterationB_;
    tick_t tick_;
    bool tick_empty_;
    
};

}

#endif
