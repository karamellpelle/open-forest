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
        while ( !stack_.empty() )
        {
            Iteration<A>::release( stack_.top() );
            stack_.pop();
        }
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

