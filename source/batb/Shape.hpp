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
#ifndef BATB_SHAPE_HPP
#define BATB_SHAPE_HPP
#include "batb/batb_include.hpp"

namespace batb
{


// normalized size
class Shape
{
public:
    Shape() : Shape( 0.0, 0.0 )                       { }
    Shape(float_t w, float_t h) : wth( w ), hth( h )  { }

    float_t wth;
    float_t hth;


    // set shape from size
    void size(uint w, uint h)
    {
        float_t n = (float_t)( std::max( w, h ) );
        wth = (float_t)( w ) / n;
        hth = (float_t)( h ) / n;
    }
};


}



#endif
