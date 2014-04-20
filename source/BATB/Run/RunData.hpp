#ifndef BATB_RUNDATA_HPP
#define BATB_RUNDATA_HPP
#include "BATB/include.hpp"
#include "BATB/Run/RunPrim.hpp"


namespace BATB
{


class RunData
{
public:
    static void create();
    static void destroy();
    static RunData* theRunData() { return &theRunData_; }

    // 
    RunPrim* prim;


private:
    static RunData theRunData_;
    static RunPrim theRunPrim_;

    RunData()
    {

    }

};


}


#endif

