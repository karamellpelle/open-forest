#ifndef GAME_DO_HPP
#define GAME_DO_HPP

namespace Game
{

template <typename A>
class Do : public Ref
{
public:
    virtual ~Do() { }


    virtual void doWorld(A& a) = 0;



}

}

#endif
