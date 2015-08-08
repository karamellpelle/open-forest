//    open-demo: an orientering game.
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
#include "batb.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <random>
#include "helpers/bezier.hpp"

namespace batb
{

namespace demo
{



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IterationDemoForest::IterationDemoForest(BATB& b) : IterationDemo( b )
{

}



void IterationDemoForest::iterate_begin(World& demo)
{
    BATB_LOG_FUNC( batb );

}


IterationStack IterationDemoForest::iterate_demo(World& demo)
{
    // continue with this iteration
    return { this };

}


////////////////////////////////////////////////////////////////////////////////
//
//
void begin(IterationDemoForest& iter)
{
    BATB_LOG_FUNC( iter.batb );


}

void end(IterationDemoForest& iter)
{
    BATB_LOG_FUNC( iter.batb );
}

////////////////////////////////////////////////////////////////////////////////
// 
//



} // namespace demo

} // namespace batb


