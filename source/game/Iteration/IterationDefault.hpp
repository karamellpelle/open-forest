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
#ifndef GAME_ITERATION_DEFAULT_HPP
#define GAME_ITERATION_DEFAULT_HPP
#include "game/Iteration.hpp"
#include "game/Output.hpp"
#include "game/Step.hpp"


namespace game
{


template <typename A>
class IterationDefault : public Iteration<A>
{
public:
    static IterationDefault<A>* create(Output<A>* out, Step<A>* step) { return new IterationDefault( out, step ); }
    static void destroy(IterationDefault<A>* iter)                    { iter->destroy(); }

    void iterate(IterationStack<A>& stack, A& a)
    {
        output_->outputWorld( a );
        step_->stepWorld( this, stack, a );
    }

protected:
    IterationDefault(Output<A>* out, Step<A>* step) : output_( out ), step_( step )
    {
        Ref::hold( output_ );
        Ref::hold( step_ );
    }
    ~IterationDefault()
    {
        Ref::release( step_ );
        Ref::release( output_ );
    }
    void destroy()
    {
        delete this;
    }

private:
    Output<A>* output_;
    Step<A>* step_;

};


}

#endif
