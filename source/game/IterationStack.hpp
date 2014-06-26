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
#ifndef ITERATION_STACK_HPP
#define ITERATION_STACK_HPP
#include <stack>
#include <vector>
#include "GameInclude.hpp"
#include "Iteration.hpp"


namespace Game
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

    void push() { }
    void push(Iteration<A>* n0) { push_( n0 ); } 
    void push(Iteration<A>* n0, Iteration<A>* n1) { push_( n1 ); push_( n0 ); }
    void push(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2) { push_( n2 ); push_( n1 ); push_( n0 );}
    void push(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3) { push_( n3 ); push_( n2 ); push_( n1 ); push_( n0 ); }
    void push(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4) { push_( n4 ); push_( n3 ); push_( n2 ); push_( n1 ); push_( n0 ); }
    void push(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4, Iteration<A>* n5) { push_( n5 ); push_( n4 ); push_( n3 ); push_( n2 ); push_( n1 ); push_( n0 ); }
    void push(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4, Iteration<A>* n5, Iteration<A>* n6) { push_( n6 ); push_( n5 ); push_( n4 ); push_( n3 ); push_( n2 ); push_( n1 ); push_( n0 ); }
    void push(Iteration<A>* n0, Iteration<A>* n1, Iteration<A>* n2, Iteration<A>* n3, Iteration<A>* n4, Iteration<A>* n5, Iteration<A>* n6, Iteration<A>* n7) { push_( n7 ); push_( n6 ); push_( n5 ); push_( n4 ); push_( n3 ); push_( n2 ); push_( n1 ); push_( n0 ); }

    
    bool empty() const { return stack_.empty(); }
    void iterate(A& a)
    {
        if ( !stack_.empty() )
        {
            Iteration<A>* iteration = stack_.top();
            stack_.pop();

            // make 1 iteration on 'a'
            iteration->iterate( *this, a );

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
    void push_(Iteration<A>* iteration)
    {
        Ref::hold( iteration );
        stack_.push( iteration );
    }

    std::stack< Iteration<A>*, std::vector< Iteration<A>* > > stack_;

    
};

}

#endif

