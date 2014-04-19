#ifndef GAME_NO_OUTPUT_HPP
#define GAME_NO_OUTPUT_HPP
#include "../Output.hpp"


namespace Game
{


template <typename A>
class NoOutput : public Output<A>
{
public:
    static NoOutput<A>* create()
    {
        static NoOutput ret;
        return &ret;
    }
    static void destroy(NoOutput<A>* out) { }


    void outputWorld(A& a) { } 

private:
    NoOutput() { }

};

}

#endif
