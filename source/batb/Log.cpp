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
#include "batb/Log.hpp"

////////////////////////////////////////////////////////////////////////////////
//  


namespace batb
{

Log::Log() : std::ostream( this )
{

}

std::ostream& Log::endl()
{
    return operator<<( std::endl );
}

std::ostream& Log::flush()
{
    return operator<<( std::endl );
}


std::streamsize Log::xsputn(const char* s, std::streamsize n)
{
    //if ( s == '\n' ) indent 

    // TMP: just write to std::cout
    return std::cout.rdbuf()->sputn( s, n );

    
}

int Log::overflow (int c)
{
    // TMP: just write to std::cout
    return std::cout.rdbuf()->sputc( c );
}



} // namespace batb
