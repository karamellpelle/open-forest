//    open-demo: an orientering game.
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
#include "BATB/Demo.hpp" 

namespace batb
{



namespace demo
{


////////////////////////////////////////////////////////////////////////////////
//  Demo

Demo::Demo(BATB* b) : ModuleBATB( b )
{
    keys = std::make_unique<KeySet>( b );
    events = std::make_unique<EventList>();
}



////////////////////////////////////////////////////////////////////////////////
// 
void Demo::begin(const std::string& path)
{
    batb->log << "batb->demo->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );
    
    if ( init_empty() )
    {
        config( path );

        // load associated keys 
        if ( auto keyset = yaml["KeySet"] )
        {
            batb->log << "loading KeySet: " << std::endl;
            LogIndent indent( batb->log, "- " );

            keys->load( keyset );
        }

    }

    init( true );
}

void Demo::end()
{
    batb->log << "batb->demo->end()" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_nonempty() )
    {
        save();

    }
    
    init( false );
}


} // namespace demo

} // namespace batb


