#ifndef GAME_THINK_HPP
#define GAME_THINK_HPP


namespace Game
{

template <typename A>
class Think : public Ref
{
public:
    virtual ~Think() { } 

    virtual void thinkWorld(IterationStack<A>& stack, A& a) = 0;
    

};


}

#endif
