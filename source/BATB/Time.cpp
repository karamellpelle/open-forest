//    open-forest: an orienteering game.
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
#include "BATB/Time.hpp"


namespace batb
{
namespace time
{


////////////////////////////////////////////////////////////////////////////////
// setup

void Time::begin(const std::string& path)
{
    batb->log << "batb->time->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );


    if ( init_empty() )
    {
        // set configuration file
        config( path );

        ////////////////////////////////////////////////////////////////////////////////
        // setup Time
        //

    }

    init( true );
}

void Time::end()
{
    batb->log << "batb->time->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        save();


    }

    init( false );
}

////////////////////////////////////////////////////////////////////////////////
// frame
//

void Time::frameBegin()
{
    tick_ = getNow();
}

void Time::frameEnd()
{

}




} // namespace time
} // namespace batb


