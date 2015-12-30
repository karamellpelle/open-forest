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
#include "batb/value/batb.hpp"
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
        ////////////////////////////////////////////////////////////////////////////////

        forestAimX =        4.0;
        forestAimY =        4.0;
        forestMoveX =       78.0;
        forestMoveZ =       78.0;

        forestProximityControl  = 16.0;
        forestProximityRunner   = 16.0;

        forestTraceD =      16.0;

        forestModifyRunnerSpeed = 35.0;

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

float_t forestModifyRunnerSpeed;

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

} // namespace value

} // namespace batb

