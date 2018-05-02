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
#include "BATB/Value.hpp"

// values are declared here:
#include "BATB/Value/Forest.hpp"
#include "BATB/Value/Run.hpp"
#include "BATB/Value/tb.hpp"
#include "BATB/Value/BATB.hpp"


namespace batb
{

namespace value
{


////////////////////////////////////////////////////////////////////////////////
//  

void Value::begin(const std::string& path)
{
    batb->log << "batb->value->begin( " << path << " )" << std::endl;
    LogIndent indent( batb->log, "* " );

    if ( init_empty() )
    {
        // set configuration file
        config( path );

        ////////////////////////////////////////////////////////////////////////////////
        // TODO: use 'yaml' to override values, if defined

        batb->log << "(TODO: read values from yaml node)" << std::endl;
    }
    init( true );
}

void Value::end()
{
    batb->log << "batb->value->end()" << std::endl;
    LogIndent indent( batb->log, "* " );
    if ( init_nonempty() )
    {
        // save the configuration to its file
        save();
        
    }
    init( false );
}



////////////////////////////////////////////////////////////////////////////////
//  set/get values

bool Value::set(const std::string& name, const std::string& v)
{
#define TRY_SET_VALUE( variable ) if ( name == #variable ) return (bool)(is >> (variable)) // TODO: error message

    std::istringstream is( v ); 

    TRY_SET_VALUE( dt );
    TRY_SET_VALUE( dt_max );
    TRY_SET_VALUE( runConsoleOpenTicks );
    TRY_SET_VALUE( runConsoleOpacity );
    TRY_SET_VALUE( forestAimX );
    TRY_SET_VALUE( forestAimY );
    //TRY_SET_VALUE( forestMoveX );
    //TRY_SET_VALUE( forestMoveZ );
    TRY_SET_VALUE( forestProximityControl );
    TRY_SET_VALUE( forestProximityRunner );
    TRY_SET_VALUE( forestTraceD );
    TRY_SET_VALUE( forestModifyRunnerSpeed );
    TRY_SET_VALUE( forestRunnerAnimSpeed );
    TRY_SET_VALUE( forestRunnerAnimSpeedMin );
    

    batb->log << "Value: could not find variable '" << name << "'" << std::endl;
    return false;
}

bool Value::get(const std::string& name, std::string& v)
{
#define TRY_GET_VALUE( variable ) if ( name == #variable ) { os << (variable); v = os.str(); return true; }

    std::ostringstream os;
    TRY_GET_VALUE( dt );
    TRY_GET_VALUE( dt_max );
    TRY_GET_VALUE( runConsoleOpenTicks );
    TRY_GET_VALUE( runConsoleOpacity );
    TRY_GET_VALUE( forestAimX );
    TRY_GET_VALUE( forestAimY );
    //TRY_GET_VALUE( forestMoveX );
    //TRY_GET_VALUE( forestMoveZ );
    TRY_GET_VALUE( forestProximityControl );
    TRY_GET_VALUE( forestProximityRunner );
    TRY_GET_VALUE( forestTraceD );
    TRY_GET_VALUE( forestModifyRunnerSpeed );
    TRY_GET_VALUE( forestRunnerAnimSpeed );
    TRY_GET_VALUE( forestRunnerAnimSpeedMin );

    batb->log << "Value: could not find variable '" << name << "'" << std::endl;
    return false;
}



} // namespace value

} // namespace batb

