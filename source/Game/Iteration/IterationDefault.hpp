#ifndef ITERATION_DEFAULT_HPP
#define ITERATION_DEFAULT_HPP
#include "../Iteration.hpp"
#include "../Output.hpp"
#include "../Step.hpp"


namespace Game
{


template <typename A>
class IterationDefault : public Iteration<A>
{
public:
    static IterationDefault<A>* create(Output<A>* out, Step<A>* step) { return new IterationDefault( out, step ); }
    static void destroy(IterationDefault<A>* iter)                    { delete iter; }

    void iterate(IterationStack<A>& stack, A& a)
    {
        output_->outputWorld( a );
        step_->stepWorld( this, stack, a );
    }

    // let IterationDefault handle Output and Step primitives
    static Output<A>* releasing(Output<A>* output)  { Ref::releasing(output); return output; }
    static Step<A>*   releasing(Step<A>* step)      { Ref::releasing(step); return step; }

protected:
    IterationDefault(Output<A>* out, Step<A>* step) : output_( out ), step_( step )
    {
        Ref::hold( step );
        Ref::hold( step );
    }
    ~IterationDefault()
    {
        Ref::release( step_ );
        Ref::release( output_ );
    }
    void destroy_()
    {
        delete this;
    }

private:
    Output<A>* output_;
    Step<A>* step_;

};


}

#endif
