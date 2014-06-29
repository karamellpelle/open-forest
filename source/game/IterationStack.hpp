//    orienteerng-game: an orientering game.
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
#ifndef GAME_ITERATION_STACK_HPP
#define GAME_ITERATION_STACK_HPP
#include <stack>
#include <vector>
#include "game/Iteration.hpp"


namespace game
{


template <typename A>
class IterationStack
{
public:
    IterationStack() { }
    ~IterationStack()
    {
        clear();
    }

    void next() { }
    void next(Iteration<A>* n0) { next_( n0 ); } 
    void next(Iteration<A>* n0, Iteration<A>* n1) { next_( n1 ); next_( n0 ); }
    void next(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2) { next_( n2 ); next_( n1 ); next_( n0 );}
    void next(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3) { next_( n3 ); next_( n2 ); next_( n1 ); next_( n0 ); }
    void next(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4) { next_( n4 ); next_( n3 ); next_( n2 ); next_( n1 ); next_( n0 ); }
    void next(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4, Iteration<A>* n5) { next_( n5 ); next_( n4 ); next_( n3 ); next_( n2 ); next_( n1 ); next_( n0 ); }
    void next(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4, Iteration<A>* n5, Iteration<A>* n6) { next_( n6 ); next_( n5 ); next_( n4 ); next_( n3 ); next_( n2 ); next_( n1 ); next_( n0 ); }
    void next(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4, Iteration<A>* n5, Iteration<A>* n6, Iteration<A>* n7) { next_( n7 ); next_( n6 ); next_( n5 ); next_( n4 ); next_( n3 ); next_( n2 ); next_( n1 ); next_( n0 ); }
    void finish() { next(); }

    
    bool empty() const { return stack_.empty(); }
    void iterate(A& a)
    {
        if ( !stack_.empty() )
        {
            Iteration<A>* iteration = stack_.top();
            stack_.pop();

            // make 1 iteration on 'a'
            iteration->iterate( *this, a );

            // autorelease 'iteration', if no more references to it
            Ref::release( iteration );

        }
    }
    void clear()
    {
        while ( !stack_.empty() )
        {
            Iteration<A>::release( stack_.top() );
            stack_.pop();
        }
    }


private:
    // add iteration to this stack, 
    void push_(Iteration<A>* iteration)
    {
        stack_.push( iteration );

        // hold increase number of references to 'iteration'
        Ref::hold( iteration );
    }

    std::stack< Iteration<A>*, std::vector< Iteration<A>* > > stack_;

    
};

}

#endif

