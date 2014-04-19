#ifndef ITERATION_STEP_DEFAULT_HPP
#define ITERATION_STEP_DEFAULT_HPP
#include "Game/Iteration.hpp"
#include "Game/Do.hpp"
#include "Game/Think.hpp"


namespace Game
{


template <typename A>
class StepDefault : public Step<A>
{
public:
    static StepDefault<A>* create(Do<A>* d, Think<A>* t) { return new StepDefault( d, t ); }
    static void destroy(StepDefault<A>* iter)            { delete this; }
    void stepWorld(IterationStack<A>& stack, A& a)
    {
        do_->doWorld( a );
        think_->thinkWorld( a );
        
    }

    static Do<A>*    releasing(Do<A>* ref)    { Ref<A>::releasing(ref); return ref; }
    static Think<A>* releasing(Think<A>* ref) { Ref<A>::releasing(ref); return ref; }


protected:
    StepDefault(Do<A>* d, Think<A>* t) : do_( d ), think_( t )
    {
        Ref::hold( do_ );
        Ref::hold( think_ );
    }
    ~StepDefault()
    {
        Ref::release( think_ );
        Ref::release( do_ );
    }
    void ~destroy_()
    {
        delete this;
    }

private:
    Do<A>* do_;
    Think<A>* think_;

};


}

#endif
