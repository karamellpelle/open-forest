#ifndef GAME_OUTPUT_HPP
#define GAME_OUTPUT_HPP
#include "Ref.hpp"

namespace Game
{


template <typename A>
class Output : public Ref
{
friend class Iteration<A>;
public:
    virtual ~Output() { }   // FIXME: protected??

    // we prefer named constructors and destructors, so for 
    // subclass ObjectX, make static functions:
    //
    // static OutputX* create(Y y)          { ... }
    // static void     destroy(Output<A>* ) { ... }

    virtual void outputWorld(A& a) = 0;

};


}

#endif
