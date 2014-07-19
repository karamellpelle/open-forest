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

Run::Run(BATB& b) : batb( b ), keyset( b ), 
                    iterationRunBegin( b ), 
                    iterationRunEnd( b ),
                    iterationRunMain( b )
{

}



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
        throw std::runtime_error( THIS_FUNCTION );
    }

    // TODO: parse xml...
    run.keyset.loadXML("batb/run/KeySet.xml");

    // note: iterations are set up by iterationRunBegin, instead of here,
    //       despite this Run object holds the iterations. is this wanted
    //       behaviour? IterationRunBegin/IterationRunEnd are never set up
    //       by this function. and it is nice to let iterationRunBegin load
    //       all parts of BATB in a sequence, in order to make interactive
    //       loading.
    run.initialized_ = true;
}


void end(Run& run)
{
    run.batb.log << THIS_FUNCTION << std::endl;    

    if ( run.initialized_ )
    {
        run.saveXML();
    }
    
    run.initialized_ = false;

}


} // namespace run

} // namespace batb
