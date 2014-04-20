#ifndef RUN_PRIM_HPP
#define RUN_PRIM_HPP
#include "BATB/include.hpp"
#include "BATB/Run/RunWorld.hpp"
#include "Game.hpp"



namespace BATB
{



class RunPrim
{
public:
    // TODO: custom types for each iteration!
    Game::Iteration<RunWorld>* iterationDataBegin;
    Game::Iteration<RunWorld>* iterationDataEnd;
    Game::Iteration<RunWorld>* iterationIntro;
    Game::Iteration<RunWorld>* iterationMain;
    Game::Iteration<RunWorld>* iterationOutro;

};



}
#endif
