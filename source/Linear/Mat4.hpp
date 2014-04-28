//    orienteering-game: an orientering game.
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
#ifndef LINEAR_MAT4_HPP
#define LINEAR_MAT4_HPP
#include "Linear/Linear_include.hpp"

namespace Linear
{


class Mat4
{
public:
    Mat4() : a( abcd + 0 ), b( abcd + 4 ), c( abcd + 8 ), d( abcd + 12 ) { }

    void identity()
    {
        abcd[ 0] = 1;
        abcd[ 1] = 0;
        abcd[ 2] = 0;
        abcd[ 3] = 0;
        abcd[ 4] = 0;
        abcd[ 5] = 1;
        abcd[ 6] = 0;
        abcd[ 7] = 0;
        abcd[ 8] = 0;
        abcd[ 9] = 0;
        abcd[10] = 1;
        abcd[11] = 0;
        abcd[12] = 0;
        abcd[13] = 0;
        abcd[14] = 0;
        abcd[15] = 1;
    }

    // add transformation 'b' after this
    Mat4& push_back(const Mat4& b);
    // multiply
    Mat4& scale(float_t a);

    float_t abcd[ 16 ];
    const float_t* a;
    const float_t* b;
    const float_t* c;
    const float_t* d;
    
};


inline Mat4& Mat4::push_back(const Mat4 b)
{
    abcd[ 0] = ;
    abcd[ 1] = ;
    abcd[ 2] = ;
    abcd[ 3] = ;
    abcd[ 4] = ;
    abcd[ 5] = ;
    abcd[ 6] = ;
    abcd[ 7] = ;
    abcd[ 8] = ;
    abcd[ 9] = ;
    abcd[10] = ;
    abcd[11] = ;
    abcd[12] = ;
    abcd[13] = ;
    abcd[14] = ;
    abcd[15] = ;
}

}

#endif
