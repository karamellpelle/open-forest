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
#include "BATB/Forest.hpp"
#include "BATB/Log.hpp"

namespace BATB
{



void Forest::create(xml::XMLElement* elem)
{
    using namespace xml;

    log << "Forest::create() " << std::endl;    


    XMLHandle xml( elem );
    // FIXME: parse xml...
  

  
    // create primitives
    prim->create( xml.FirstChildElement( "Prim" ).ToElement() );

    // create settings
    //settings->create( xml.FirstChildElement( "Settings" ).ToElement() );

    // create keys
    keys->create( xml.FirstChildElement( "Keys" ).ToElement() );


}


void Forest::destroy()
{
    log << "Forest::destroy() " << std::endl;    

    prim->destroy();

    keys->destroy();
}


}
