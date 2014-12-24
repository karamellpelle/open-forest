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
#include "batb.hpp"




namespace batb
{



namespace xxx
{


////////////////////////////////////////////////////////////////////////////////
//  Xxx

Xxx::Xxx(BATB& b) : batb( b ), 
{

}



void Xxx::save()
{

    // FIXME: write to file
}


////////////////////////////////////////////////////////////////////////////////
// 
void begin(Xxx& xxx)
{

    xxx.batb.log << THIS_FUNCTION << std::endl;    


    // set up this Xxx object from file
    YAML::Node yaml = YAML::LoadFile( xxx.filepath_ );

    xxx.initialized_ = true;
}


void end(Xxx& xxx)
{
    xxx.batb.log << THIS_FUNCTION << std::endl;    

    if ( xxx.initialized_ )
    {
        xxx.save();
    }
    
    xxx.initialized_ = false;

}


} // namespace xxx

} // namespace batb
