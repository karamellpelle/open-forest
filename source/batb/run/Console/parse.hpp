//    open-forest: an orientering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#ifndef BATB_RUN_CONSOLE_CONSOLE_PARSE_HPP
#define BATB_RUN_CONSOLE_CONSOLE_PARSE_HPP
#include <cctype>
#include "batb/batb_include.hpp"

  
namespace batb
{

namespace run
{

// eat spaces (trim front)
inline std::string space(std::string& str)
{
    // http://stackoverflow.com/questions/21578544/stdremove-if-and-stdisspace-compile-time-error
    auto is = [](std::string::value_type c) { return std::isspace(c); };

    auto b = std::begin( str );
    auto e = std::end( str );
    auto i = std::find_if_not( b, e, is );

    auto ret = std::string( b, i );

    str.erase( std::begin( str ), i );

    return ret;
}

// eat word
inline std::string word(std::string& str)
{
    // http://stackoverflow.com/questions/21578544/stdremove-if-and-stdisspace-compile-time-error
    auto is = [](std::string::value_type c) { return std::isspace(c); };

    // find word (trim front from spaces)
    auto b = std::find_if_not( std::begin( str ), std::end( str ), is );
    auto i = std::find_if( b, std::begin( str ), is );
    
    auto ret = std::string( b, i );
    str.erase( std::begin( str ), i );

    return ret;

}

// if given word is found (and word ended with space), remove it
inline bool word(const std::string& ref, std::string& str)
{
    // http://stackoverflow.com/questions/21578544/stdremove-if-and-stdisspace-compile-time-error
    auto is = [](std::string::value_type c) { return std::isspace(c); };

    // find word (trim front from spaces)
    auto b = std::find_if_not( std::begin( str ), std::end( str ), is );
    auto i = std::find_if( b, std::begin( str ), is );
   
    auto word = std::string( b, i );

    if ( word == ref )
    {
        str.erase( std::begin( str ), i );
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


#endif
