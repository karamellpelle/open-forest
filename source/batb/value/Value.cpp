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
#include "batb/value/Value.hpp"
#include "batb/value/forest.hpp"
#include "batb/value/run.hpp"
#include "batb/BATB.hpp"


namespace batb
{

namespace value
{


////////////////////////////////////////////////////////////////////////////////
//  Value

////////////////////////////////////////////////////////////////////////////////
//  

void begin(Value& value)
{
    if ( value.init_empty() )
    {

        // default values:
        proj3DNear =        0.1;
        proj3DFar =         512.0;
        proj3DFOVY =        1.047;

        dt =                0.02;  
        dt_max =            2.0;

        forestAimX =        4.0;
        forestAimY =        4.0;
        forestMoveX =       45.0;
        forestMoveZ =       45.0;

        forestProximityControl  = 16.0;
        forestProximityRunner   = 16.0;

        forestTraceD =      16.0;

        // TODO: use 'Value::yaml', override values, if defined
    }
    value.init( true );
}

void end(Value& value)
{
    if ( value.init_nonempty() )
    {
        // save the configuration to its file
        value.save();
        
    }
    value.init( false );
}


////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////////////////////////////////////////////
//  run.hpp
//
float_t proj3DNear;
float_t proj3DFar;
float_t proj3DFOVY;

tick_t dt;
tick_t dt_max;

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


} // namespace value

} // namespace batb

