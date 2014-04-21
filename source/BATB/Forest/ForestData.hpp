#ifndef BATB_FORESTDATA_HPP
#define BATB_FORESTDATA_HPP
#include "BATB/Forest/ForestPrim.hpp"
#include "BATB/Forest/ForestSettings.hpp"

namespace BATB
{


class ForestData
{
public:
    static void create();
    static void destroy();
    static ForestData* theForestData() { return &theForestData_; }

    ForestPrim* prim;
    ForestSettings* settings;
    


private:
    static ForestData theForestData_;
    static ForestPrim theForestPrim_;
    static ForestSettings theForestSettings_;

    ForestData()
    {

    }

};


}


#endif

