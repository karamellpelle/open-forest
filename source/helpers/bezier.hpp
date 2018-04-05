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
#ifndef HELPERS_BEZIER_HPP
#define HELPERS_BEZIER_HPP
#include "include.hpp"
#include <vector>



// TODO: make this template general for more container types
template <typename A, typename Container>
typename Container::value_type bezier(const Container& points, const A& a)
{
    // here, we assume size is non-empty

    // copy
    // TODO: std::array instead of copy
    Container ps = points;

    uint n = ps.size();
    while ( n != 0 )
    {
        for (uint i = 0; i + 1 != n; ++i)
        {
            // interpolate p0 -> p1
            ps[ i ] *= (1 - a);
            ps[ i ] += a * ps[ i + 1 ];
        }
        --n;
    }

    return ps[0];
}



#endif

