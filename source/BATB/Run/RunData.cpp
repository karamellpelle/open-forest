#include "BATB/Run/RunData.hpp"
#include "BATB/Log.hpp"


namespace BATB
{


RunData RunData::theRunData_;
RunPrim RunData::theRunPrim_;


void RunData::create()
{
    log << "RunData::create() " << std::endl;    
  
    // create primitives
    RunPrim* prim = &theRunPrim_;
    prim->iterationDataBegin =  Game::NoIteration<RunWorld>::create();
    prim->iterationDataEnd   =  Game::NoIteration<RunWorld>::create();
    prim->iterationIntro     =  Game::NoIteration<RunWorld>::create();
    prim->iterationMain      =  Game::NoIteration<RunWorld>::create();
    prim->iterationOutro     =  Game::NoIteration<RunWorld>::create();

    // point to primitives
    theRunData_.prim = prim;


}


void RunData::destroy()
{
    log << "RunData::destroy() " << std::endl;    

}

};
