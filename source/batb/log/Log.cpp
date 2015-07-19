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
#include "batb/log/Log.hpp"


namespace batb
{

namespace log
{

////////////////////////////////////////////////////////////////////////////////
//  

std::streamsize LogStreamBuf::xsputn(const char* s, std::streamsize n)
{
    //if ( s == '\n' ) indent 

    // TMP:
    return std::cout.rdbuf()->sputn( s, n );

    
}

int LogStreamBuf::overflow (int c)
{
    // TMP:
    return std::cout.rdbuf()->sputc( c );
}

////////////////////////////////////////////////////////////////////////////////
//  
void begin(Log& log)
{
    if ( log.ModuleBATB::init_empty() )
    {

    }

    log.ModuleBATB::init( true );
}

void end(Log& log)
{
    if ( log.ModuleBATB::init_nonempty() )
    {
        log.save();
    }
    log.ModuleBATB::init( false );
}


} // namespace log

} // namespace batb
