#ifndef GAME_NO_ITERATION_HPP
#define GAME_NO_ITERATION_HPP
#include "Game/Iteration.hpp"


namespace Game
{


template <typename A>
class NoIteration : public Iteration<A>
{
public:
    static NoIteration<A>* create()
    {
        static NoIteration ret;
        return &ret;
    }

    void iterate(IterationStack<A>& stack, A& a) { } 

private:
    NoIteration() { }

};

}

#endif
