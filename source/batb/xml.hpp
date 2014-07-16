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
#ifndef BATB_XML_HPP
#define BATB_XML_HPP
#include "batb/xml/XML.hpp"
#include "batb/xml/helpers.hpp"

////////////////////////////////////////////////////////////////////////////////
// nanovg
#include "nanovg.h"


namespace batb
{

namespace xml
{

// rename types into our namespace
typedef tinyxml2::XMLError          Error;
typedef tinyxml2::XMLVisitor        Visitor;
typedef tinyxml2::XMLNode	    Node;
typedef tinyxml2::XMLText	    Text;
typedef tinyxml2::XMLComment	    Comment;
typedef tinyxml2::XMLDeclaration    Declaration;
typedef tinyxml2::XMLUnknown	    Unknown;
typedef tinyxml2::XMLAttribute	    Attribute;
typedef tinyxml2::XMLElement	    Element;
typedef tinyxml2::XMLDocument	    Document;
typedef tinyxml2::XMLHandle	    Handle;
typedef tinyxml2::XMLConstHandle    ConstHandle;
typedef tinyxml2::XMLPrinter        Printer;


// export the rest (actually all) (values, ...)
using namespace tinyxml2;


} // namespace xml

} // namespace batb

#endif
