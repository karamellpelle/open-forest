#ifndef BATB_LOG_HPP
#define BATB_LOG_HPP
#include "BATB/include.hpp"
#include <iostream>


namespace BATB
{

// FIXME: implement Log as custom ostream class, adding 'BATB :    "-prefix for each BATB::log!
extern std::ostream& log;


class Log
{
public:
    static void create() { }
    static void destroy() { }

};

}

#endif
