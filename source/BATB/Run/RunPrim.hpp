#ifndef RUN_PRIM_HPP
#define RUN_PRIM_HPP
#include "BATB/include.hpp"
#include "BATB/Run/RunWorld.hpp"
#include "BATB/Run/Prim/Iteration.hpp"
#include "Game.hpp"



namespace BATB
{



class RunPrim
{
public:
    // TODO: custom types for each iteration!
    IterationRunWorld* iterationDataBegin;
    IterationRunWorld* iterationDataEnd;
    IterationRunWorld* iterationIntro;
    IterationRunWorld* iterationMain;
    IterationRunWorld* iterationOutro;

};



}
#endif
