#ifndef OLD_HPP
#define OLD_HPP
#include <iostream>
#include <string>

inline std::string old_file(const char* path)
{
    // executable is built into "build/" folder, point to data dir
    static std::string prefix = "../data/old/"

    return prefix + path;
}



#endif
