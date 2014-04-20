#ifndef BATB_FORESTDATA_HPP
#define BATB_FORESTDATA_HPP


namespace BATB
{


class ForestData
{
public:
    static void create();
    static void destroy();
    static ForestData* theForestData() { return &theForestData_; }

    // 



private:
    static ForestData theForestData_;

    ForestData()
    {

    }

};


}


#endif

