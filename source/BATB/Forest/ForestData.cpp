#include "include.hpp"
#include "BATB/Log.hpp"

namespace BATB
{

ForestData ForestData::theForestData_;
ForestPrim ForestData::theForestPrim_;
ForestSettings ForestData::theForestSettings_;


void ForestData::create()
{
    // FIXME: from file/config?
  
    ForestData* this_ = &theForestData_;

    log << "ForestData::create() " << std::endl;    
  
    // create primitives
    this_->prim = &theForestPrim_;
    //this_->prim-> = 

    // create settings
    this_->settings = &theForestSettings_;
    //this_->settings->


}


void ForestData::destroy()
{
    log << "ForestData::destroy() " << std::endl;    

}


}
