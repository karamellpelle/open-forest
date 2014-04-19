#ifndef ITERATION_FUNCTION_HPP
#define ITERATION_FUNCTION_HPP
#include "../Iteration.hpp"

namespace Game
{


// create 'Iteration' from function
template <typename A>
class IterationFunction : public Iteration<A>
{
public:
    typedef void (*FunctionT)(Iteration<A>* , IterationStack<A>& , A& );


    // create/destroy
    static IterationFunction<A>* create(FunctionT f) { return new IterationFunction( f ); }   // create
    static void destroy(IterationFunction<A>* iter)  { delete iter; }                        // free mem, for user-release

    void iterate(IterationStack<A>& stack, A& a) { function_(this, stack, a); }

protected:
    IterationFunction(FunctionT f) : function_( f ) { }
    ~IterationFunction() { } 
    void destroy_()
    {
        delete this;
    }

private:

    FunctionT function_;

};


}

#endif
