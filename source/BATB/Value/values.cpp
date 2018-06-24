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
//  run.hpp
//
float_t proj3DNear               = 0.1;
float_t proj3DFar                = 512.0;
float_t proj3DFOVY               = 1.047;

tick_t  dt                       = 0.02;
tick_t  dt_max                   = 2.0;

float_t runConsoleOpenTicks      = 0.3;
float_t runConsoleOpacity        = 0.55;


////////////////////////////////////////////////////////////////////////////////
//  forest.hpp
//

float_t forestAimX               = 4.0;
float_t forestAimY               = 4.0;
float_t forestMoveX              = 4000.0;
float_t forestMoveZ              = 4000.0;
float_t forestCameraMoveXTicks   = 6.0;
float_t forestCameraMoveXMin     = 78.0;
float_t forestCameraMoveXMax     = 144.0;
float_t forestCameraMoveZTicks   = 10.0;
float_t forestCameraMoveZMin     = 32.0;
float_t forestCameraMoveZMax     = 4000.0;

float_t forestProximityControl   = 2500.0; // 50 meters
float_t forestProximityRunner    = 8100.0; // 90 meters

float_t forestTraceD             = 4.0;

float_t forestModifyRunnerSpeed  = 35.0;

float_t forestRunnerAnimSpeed    = 0.032;
float_t forestRunnerAnimSpeedMin = 1.0;


////////////////////////////////////////////////////////////////////////////////
//  BATB.hpp
//

float_t coursedrawerSizeNormal   = 1.00;
float_t coursedrawerSizeStart    = 1.41;
float_t coursedrawerSizeFinishA  = 0.83;
float_t coursedrawerSizeFinishB  = 1.17;
float_t coursedrawerSizeIx       = 2.0 * 1.3;
float_t coursedrawerSizeIxPad    = 2.44;
float_t coursedrawerSizeLine     = 0.16;

float_t coursedrawerColorR       = 0.51;
float_t coursedrawerColorG       = 0.04;
float_t coursedrawerColorB       = 0.35;
float_t coursedrawerColorA       = 0.6;

////////////////////////////////////////////////////////////////////////////////
// tib.hpp
//
tick_t  notifyInflate            = 0.5;


////////////////////////////////////////////////////////////////////////////////
// keys.hpp

float_t keyMouseScrollUnit       = 50.0;
float_t keyMouseScrollVel        = 128.0;



} // namespace value

} // namespace batb

