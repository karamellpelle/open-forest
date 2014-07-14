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
#ifndef BATB_VALUE_VALUE_HPP
#define BATB_VALUE_VALUE_HPP
#include "batb/batb_include.hpp"


namespace batb
{

class BATB;

namespace value
{


class Value
{
friend void begin(Value& value);
friend void end(Value& value);

public:
    Value(BATB& b) : batb( b )
    {
    }

    void filepath(const std::string& path)
    {
        filepath_ = path;
    }
    void saveXML();

    BATB& batb;


private:
    bool initialized_ = false;
    
    std::string filepath_;
};

////////////////////////////////////////////////////////////////////////////////
//  
//
void begin(Value& );

void end(Value& );


////////////////////////////////////////////////////////////////////////////////
// values


// 3D viewing transformations
extern float_t proj3DNear;
extern float_t proj3DFar;
extern float_t proj3DFOVY;


} // namespace value

} // namespace batb

#endif