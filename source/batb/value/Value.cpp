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
#include "batb/value/Value.hpp"
#include "batb/BATB.hpp"

// values are declared here:
#include "batb/value/forest.hpp"
#include "batb/value/run.hpp"
#include "batb/value/tb.hpp"
#include "batb/value/batb.hpp"


namespace batb
{

namespace value
{


////////////////////////////////////////////////////////////////////////////////
//  

void Value::begin(const std::string& path)
{
    if ( init_empty() )
    {
        
        // set configuration file
        config( path );

        ////////////////////////////////////////////////////////////////////////////////
        // default values:
        // TODO: use 'yaml' to override values, if defined

        //
        proj3DNear =        0.1;
        proj3DFar =         512.0;
        proj3DFOVY =        1.047;

        dt =                0.02;  
        dt_max =            2.0;

        runConsoleOpenTicks       = 0.3;
        runConsoleOpacity         = 0.55;
        ////////////////////////////////////////////////////////////////////////////////

        forestAimX =        4.0;
        forestAimY =        4.0;
        forestMoveX =       78.0;
        forestMoveZ =       78.0;

        forestProximityControl  = 8000.0;
        forestProximityRunner   = 8000.0;

        forestTraceD =      4.0;

        forestModifyRunnerSpeed = 35.0;
        //forestModifyRunnerSpeed = 200.0;

        forestRunnerAnimSpeed     = 0.032;
        forestRunnerAnimSpeedMin  = 1.0;

        ////////////////////////////////////////////////////////////////////////////////
        
        coursedrawerSizeNormal    = 1.00;
        coursedrawerSizeStart     = 1.41;
        coursedrawerSizeFinishA   = 0.83;
        coursedrawerSizeFinishB   = 1.17;
        coursedrawerSizeIx        = 2.0 * 1.3;
        coursedrawerSizeIxPad     = 2.44;
        coursedrawerSizeLine      = 0.16;

        coursedrawerColorR        = 0.51;
        coursedrawerColorG        = 0.04;
        coursedrawerColorB        = 0.35;
        coursedrawerColorA        = 0.6;

        ////////////////////////////////////////////////////////////////////////////////
        notifyInflate             = 0.5;

    }
    init( true );
}

void Value::end()
{
    if ( init_nonempty() )
    {
        // save the configuration to its file
        save();
        
    }
    init( false );
}


////////////////////////////////////////////////////////////////////////////////
// TODO: move these into their own files

////////////////////////////////////////////////////////////////////////////////
//  run.hpp
//
float_t proj3DNear;
float_t proj3DFar;
float_t proj3DFOVY;

tick_t dt;
tick_t dt_max;

float_t runConsoleOpenTicks;
float_t runConsoleOpacity;

////////////////////////////////////////////////////////////////////////////////
//  forest.hpp
//
float_t forestAimX;
float_t forestAimY;
float_t forestMoveX;
float_t forestMoveZ;

float_t forestProximityControl;
float_t forestProximityRunner;

float_t forestTraceD;

float_t forestModifyRunnerSpeed;

float_t forestRunnerAnimSpeed;
float_t forestRunnerAnimSpeedMin;

////////////////////////////////////////////////////////////////////////////////
//  batb.hpp
//

float_t coursedrawerSizeNormal;
float_t coursedrawerSizeStart;
float_t coursedrawerSizeFinishA;
float_t coursedrawerSizeFinishB;
float_t coursedrawerSizeIx;
float_t coursedrawerSizeIxPad;
float_t coursedrawerSizeLine;

float_t coursedrawerColorR;
float_t coursedrawerColorG;
float_t coursedrawerColorB;
float_t coursedrawerColorA;

////////////////////////////////////////////////////////////////////////////////
//  tb.hpp
//
tick_t notifyInflate;

////////////////////////////////////////////////////////////////////////////////
//  Value

bool Value::set(const std::string& name, const std::string& v)
{
#define TRY_SET_VALUE( variable ) if ( name == #variable ) return (bool)(is >> (variable))

    std::istringstream is( v ); 

    TRY_SET_VALUE( dt );
    TRY_SET_VALUE( dt_max );
    TRY_SET_VALUE( runConsoleOpenTicks );
    TRY_SET_VALUE( runConsoleOpacity );
    TRY_SET_VALUE( forestAimX );
    TRY_SET_VALUE( forestAimY );
    TRY_SET_VALUE( forestMoveX );
    TRY_SET_VALUE( forestMoveZ );
    TRY_SET_VALUE( forestProximityControl );
    TRY_SET_VALUE( forestProximityRunner );
    TRY_SET_VALUE( forestTraceD );
    TRY_SET_VALUE( forestModifyRunnerSpeed );
    TRY_SET_VALUE( forestRunnerAnimSpeed );
    TRY_SET_VALUE( forestRunnerAnimSpeedMin );
    

    batb->log << "Value: could not set variable '" << name << "' to " << v << std::endl;
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
    TRY_GET_VALUE( forestMoveX );
    TRY_GET_VALUE( forestMoveZ );
    TRY_GET_VALUE( forestProximityControl );
    TRY_GET_VALUE( forestProximityRunner );
    TRY_GET_VALUE( forestTraceD );
    TRY_GET_VALUE( forestModifyRunnerSpeed );
    TRY_GET_VALUE( forestRunnerAnimSpeed );
    TRY_GET_VALUE( forestRunnerAnimSpeedMin );

    return false;
}



} // namespace value

} // namespace batb

