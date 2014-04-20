#include "include.hpp"
#include "BATB.hpp"

namespace BATB
{



void begin()
{
    // create log stream
    Log::create();

    RunData::create();
}

void end()
{
    RunData::destroy();

}


}

