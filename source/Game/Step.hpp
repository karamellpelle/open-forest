#ifndef ITERATION_STEP_HPP
#define ITERATION_STEP_HPP
#include "Ref.hpp"
#include "Iteration.hpp"


namespace Game
{


template <typename A>
class Step : public Ref
{
friend class Iteration<A>;
public:
    virtual ~Step() { }       // FIXME: protected?

    // we prefer named constructors, so for subclass IterationX, 
    // make static 'create' functions:
    //  static OutputX* create(Y y)

    virtual void stepWorld(Iteration<A>* iter, IterationStack<A>& stack, A& a) = 0;


protected:
    Step() : iteration_( 0 ) { } 

    // if this belongs to som Iteration
    Iteration<A>* iteration_;  


};


}

#endif
