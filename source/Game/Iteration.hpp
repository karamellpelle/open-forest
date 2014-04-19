#ifndef ITERATION_HPP
#define ITERATION_HPP
#include "GameInclude.hpp"
#include "Ref.hpp"



namespace Game
{

template <typename A>
class IterationStack;


template <typename A>
class Iteration : public Ref
{
friend class IterationStack<A>;
public:
    virtual ~Iteration() { }                  // FIXME: proctected, for named destructors?


    // we prefer named constructors and destructors, so for 
    // subclass IterationX, make static functions:
    //
    // static IterationX<A>*  create(Y y)             { ... }
    // static void            destroy(Iteration<A>* ) { ... }

    // define this in subclass:
    virtual void iterate(IterationStack<A>& stack, A& a) = 0;



};

}

#endif


