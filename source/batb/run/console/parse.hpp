//    open-forest: an orientering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
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
    auto e = std::end( str );
    auto i = std::find_if( b, e, is );
    
    auto ret = std::string( b, i );
    str.erase( std::begin( str ), i );

    return ret;

}

// eat alphanumeric word
inline std::string alphanums(std::string& str)
{
    // http://stackoverflow.com/questions/21578544/stdremove-if-and-stdisspace-compile-time-error
    auto is = [](std::string::value_type c) { return std::isspace(c); };
    auto ia = [](std::string::value_type c) { return std::isalnum(c); };

    // find word (trim front from spaces)
    auto b = std::find_if_not( std::begin( str ), std::end( str ), is );
    auto e = std::end( str );
    auto i = std::find_if_not( b, e, ia );
    
    auto ret = std::string( b, i );
    str.erase( std::begin( str ), i );

    return ret;

}

// alphanum + '_'
inline std::string identifier(std::string& str)
{
    // http://stackoverflow.com/questions/21578544/stdremove-if-and-stdisspace-compile-time-error
    auto is = [](std::string::value_type c) { return std::isspace(c); };
    auto ia = [](std::string::value_type c) { return std::isalnum(c) || c == '_'; };

    // find word (trim front from spaces)
    auto b = std::find_if_not( std::begin( str ), std::end( str ), is );
    auto e = std::end( str );
    auto i = std::find_if_not( b, e, ia );
    
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
    auto e = std::end( str );
    auto i = std::find_if( b, e, is );
   
    if ( std::string( b, i ) == ref )
    {
        str.erase( std::begin( str ), i );
        return true;
    }
    return false;
}


inline bool stringliteral(std::string& out, std::string& in)
{
    // trim front
    space( in );

    const auto b = std::begin( in );
    const auto e = std::end( in );
    auto i = b;

    if ( i == e ) return false;
    if ( *i != '"' ) return false; 
    ++i;

    out.clear();

    while ( i != e )
    {
        // literal complete
        if ( *i == '"' )
        {
            ++i;
            in.erase( b, i );
            return true; 
        }

        // escape character
        if ( *i == '\\'  )
        {
            ++i;
            if ( i == e ) return false;
            if ( *i == 'n' )  { out.push_back( '\n' );  ++i; continue; }
            if ( *i == 't' )  { out.push_back( '\t' );  ++i; continue; }
            if ( *i == '\\' ) { out.push_back( '\\' );  ++i; continue; }
            if ( *i == '"' )  { out.push_back( '"' );   ++i; continue; }
            
            // no special character is error:
            return false;
        }
        
        out.push_back( *i );
        ++i;
    }

    return false;
}
////////////////////////////////////////////////////////////////////////////////
//

} // namespace run

} // namespace batb


#endif
