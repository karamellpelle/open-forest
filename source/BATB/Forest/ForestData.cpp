#include "include.hpp"
#include "BATB/Log.hpp"

namespace BATB
{
ForestData ForestData::theForestData_;


void ForestData::create()
{
    log << "ForestData::create() " << std::endl;    
}


void ForestData::destroy()
{
    log << "ForestData::destroy() " << std::endl;    

}

}
