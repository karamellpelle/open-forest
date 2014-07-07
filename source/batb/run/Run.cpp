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
#include "batb.hpp"



namespace batb
{



namespace run
{


////////////////////////////////////////////////////////////////////////////////
//  Run

void Run::saveXML()
{

    xml::Document doc;
    // FIXME: populate

    //std::string errstr;
    //if ( auto err = xml::save_document( doc, filepath_, THIS_FUNCTION, errstr ) )
    //{
    //    batb.log << errstr << std::endl;
    //}
}


////////////////////////////////////////////////////////////////////////////////
// 
void begin(Run& run)
{

    run.batb.log << THIS_FUNCTION << std::endl;    


    // set up this BATB object from XML
    xml::Document doc;
    std::string errstr;
    if ( auto err = xml::load_document( doc, run.filepath_.c_str(), THIS_FUNCTION, errstr ) )
    {
        run.batb.log << errstr << std::endl;
        //return;
    }

    // TODO: parse xml...

    run.initialized_ = true;
}


void end(Run& run)
{
    run.batb.log << THIS_FUNCTION << std::endl;    

    if ( run.initialized_ )
    {

    }
    
    run.initialized_ = false;

}


} // namespace run

} // namespace batb
