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



////////////////////////////////////////////////////////////////////////////////

    // TODO: these expansions does not work. instead return an object with 
    //       variadic template operator()!

inline bool read_values(const tinyxml2::XMLElement* elem)
{
    std::cout << "read_values(elem):              ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << std::endl;

    return true;
}

template <typename T, typename... Ts>
bool read_values(const tinyxml2::XMLElement* elem, T& t, Ts&... ts)
{
    std::cout << "read_values(elem, t, ts ):      ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << ", t == " << t;
    std::cout << std::endl;


    if ( elem == nullptr )
    {
        return false; 
    }

    // read into 't':
    //if ( XMLError err = elem->QueryXXXText( &t ) )
    if ( elem->GetText() == nullptr ) // TMP
    {
        return false;
    }

    // continue with next sibling
    return read_values( elem->NextSiblingElement(), ts... );
}


template <typename... Ts>
bool read_value(const tinyxml2::XMLElement* elem, Ts&... ts)
{
    std::cout << "read_value(elem, ts):           ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << std::endl;


    // it is allowed to read an element having a value (XMLText),
    // without a T to put the value into. 
    // because 'read' can be used to check for existence of an 
    // element, i.e. Ts... is a empty set supplied to 'read'.
    return true;
}

template <typename T, typename... Ts>
bool read_value_empty(const tinyxml2::XMLElement* elem, T& t, Ts&... ts)
{
    std::cout << "read_value_empty(elem, t, ts):  ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << ", t == " << t;
    std::cout << std::endl;


    return false;
}

template <typename... Ts>
bool read_value_empty(const tinyxml2::XMLElement* elem, Ts&... ts)
{
    std::cout << "read_value_empty(elem, ts):     ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << std::endl;


    return true;
}

template <typename T, typename... Ts>
bool read_value(const tinyxml2::XMLElement* elem, T& t, Ts&... ts)
{
    std::cout << "read_value(elem, t, ts):        ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << ", t == " << t;
    std::cout << std::endl;


    if ( elem == nullptr )
    {
        return false;
    }

    // read into 't':
    //if ( XMLError err = elem->QueryXXXText( &t ) )
    if ( elem->GetText() == nullptr ) // TMP
    {
        return false;
    }

    // we have read one value, now verify that Ts... is empty:
    return read_value_empty( elem, ts... );
}

template <typename T, typename... Ts>
bool read(const tinyxml2::XMLElement* elem, T& t, Ts&... ts)
{
    std::cout << "read(elem, t, ts):              ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << ", t == " << t;
    std::cout << std::endl;


    if ( elem == nullptr )
    {
        return false;
    }

    // OK, we have expanded our Cs-path. lets see if this child 
    // contains a value (XMLText) (and, typically, no children).
    //
    // otherwise, consider the children of this child as a set S.
    // for each element s in S, s should have a value (XMLText), 
    // which we map to the respective T in Ts.

    if ( elem->GetText() )
    {
        // work on this child
        return read_value( elem, t, ts... );
    }
    else
    {
        // work on children of this child
        return read_values( elem->FirstChildElement(), t, ts... );
    }


}

template <typename C, typename... Cs, typename... Ts>
bool read(const tinyxml2::XMLElement* elem, const C& c, const Cs&... cs, Ts&... ts)
{
    std::cout << "read(elem, c, cs, ts):          ";
    if ( elem ) std::cout << "elem == " << elem->Name(); else std::cout << "elem == nullptr";
    std::cout << ", c == " << c;
    std::cout << std::endl;

    if ( elem == nullptr )
    {
        return false;
    }

    // find child child with name 'c', and continue expanding our Cs-path
    return read( elem->FirstChildElement( c ), cs..., ts... );

}


////////////////////////////////////////////////////////////////////////////////
//  reading XML.
//    currently, the result into Ts is undefined, if false return value of 'read'.
//    easy to fix this into original values, however.

template <typename C, typename... Cs, typename... Ts>
bool read(const tinyxml2::XMLElement& elem, const Cs&... cs, Ts&... ts)
{
    std::cout << "read(&elem, cs, ts):            ";
    std::cout << "elem == " << elem.Name(); 
    std::cout << std::endl;
    // work with pointers to 'XMLElement' instead:
    return read( &elem, cs..., ts... );
}


template <typename... Cs, typename... Ts>
bool read(tinyxml2::XMLDocument& doc, const Cs&... cs, Ts&... ts)
{
    std::cout << "read(&doc, cs, ts):             ";
    auto* elem = doc.RootElement();
    if ( elem ) std::cout << "doc.elem == " << elem->Name(); else std::cout << "doc.elem == nullptr";
    std::cout << std::endl;

    // work with pointers to 'XMLElement' instead:
    // note that a XML document, i.e. a XML file requires 
    // one and only one parent element (???), which we work on. 
    // this element is ignored in our Cs-path...
    return read( doc.RootElement(), cs..., ts... );

}

////////////////////////////////////////////////////////////////////////////////

} // namespace xml

} // namespace batb


#endif
