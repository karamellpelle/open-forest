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
#ifndef BATB_XML_XML_HPP
#define BATB_XML_XML_HPP
#include "batb/batb_include.hpp"


namespace batb
{

class BATB;

namespace xml
{


class XML
{
friend void begin(XML& );
friend void end(XML& );

public:
    XML(BATB& b) : batb( b )
    {
    }

    BATB& batb;

private:
    bool initialized_ = false;
};



////////////////////////////////////////////////////////////////////////////////
//  
void begin(XML& );

void end(XML& );



////////////////////////////////////////////////////////////////////////////////
//  helpers

// loading document, with error handling
tinyxml2::XMLError load_document(tinyxml2::XMLDocument& doc, const std::string& filepath, const std::string& tag, std::string& err);

// saving document, with error handling
tinyxml2::XMLError save_document(tinyxml2::XMLDocument& doc, const std::string& filepath, const std::string& tag, std::string& err);



} // namespace xml

} // namespace batb


#endif
