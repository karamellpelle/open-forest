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
#include "batb/BATB.hpp"


namespace batb
{

namespace value
{


////////////////////////////////////////////////////////////////////////////////
//  Value

void Value::save()
{
    // FIXME: populate
    YAML::Node yaml;
}

////////////////////////////////////////////////////////////////////////////////
//  

void begin(Value& value)
{
    // set up this Value object from file
    YAML::Node yaml = YAML::Load( value.filepath_ );

    // default values:
    proj3DNear =        0.1;
    proj3DFar =         512.0;
    proj3DFOVY =        1.047;

    // FIXME: now override values, if defined
    
    value.initialized_ = true;
}

void end(Value& value)
{
    if ( value.initialized_ )
    {
        // save the configuration to its file
        value.save();


        
    }
    value.initialized_ = false;
}


////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////////////////////////////////////////////
//  rendering
//
float_t proj3DNear;

float_t proj3DFar;

float_t proj3DFOVY;



} // namespace value

} // namespace batb

