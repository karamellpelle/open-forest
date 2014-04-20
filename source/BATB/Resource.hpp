#ifndef BATB_RESOURCE_HPP
#define BATB_RESOURCE_HPP
#include "BATB/include.hpp"
#include "BATB/Run/RunData.hpp"
#include "BATB/Forest/ForestData.hpp"



namespace BATB
{


inline RunData* resourceRunData()
{
    return RunData::theRunData();
}

inline ForestData* resourceForestData()
{
    return ForestData::theForestData();
}



}


#endif
