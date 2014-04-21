#include "BATB/Run/RunData.hpp"
#include "BATB/Log.hpp"


namespace BATB
{


RunData RunData::theRunData_;
RunPrim RunData::theRunPrim_;
RunSettings RunData::theRunSettings_;


void RunData::create()
{
    // FIXME: from file/config?
  
    RunData* this_ = &theRunData_;

    log << "RunData::create() " << std::endl;    
  
    // create primitives
    this_->prim = &theRunPrim_;
    this_->prim->iterationDataBegin =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationDataEnd   =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationIntro     =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationMain      =  Game::NoIteration<RunWorld>::create();
    this_->prim->iterationOutro     =  Game::NoIteration<RunWorld>::create();

    // create settings
    this_->settings = &theRunSettings_;
    //this_->settings->


}


void RunData::destroy()
{
    log << "RunData::destroy() " << std::endl;    

}

};
