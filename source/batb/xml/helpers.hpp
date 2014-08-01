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
#ifndef BATB_XML_HELPERS_HPP
#define BATB_XML_HELPERS_HPP
#include "batb/xml/XML.hpp"


namespace batb
{

class BATB;


namespace xml
{


////////////////////////////////////////////////////////////////////////////////
//  document helpers

// loading document, with error handling
tinyxml2::XMLError load_document(tinyxml2::XMLDocument& doc, const std::string& filepath, const std::string& tag, std::string& err);

// saving document, with error handling
tinyxml2::XMLError save_document(tinyxml2::XMLDocument& doc, const std::string& filepath, const std::string& tag, std::string& err);



////////////////////////////////////////////////////////////////////////////////
//

class CsElement
{
friend CsElement read(const tinyxml2::XMLElement& elem);
template <typename C, typename... Cs>
friend CsElement read(const tinyxml2::XMLElement& elem, const C& c, const Cs&... cs);
template <typename... Cs>
friend CsElement read(tinyxml2::XMLDocument& doc, const Cs&... cs);

public:

    // these are safe, all result variables are untouched 
    // upon failure, i.e. false return.
    template <typename T, typename... Ts>
    bool operator()(T& t, Ts&... ts);
    bool operator()();

    // this is unsafe, all result variables are undefined
    // upon failure, i.e. false return.
    template <typename OutputIterator>
    bool operator()(OutputIterator , OutputIterator );

    operator bool()
    {
        return elem_;  
    }
    operator const tinyxml2::XMLElement*()
    {
        return elem_;
    }

private:
    CsElement() : elem_( nullptr )                          { }
    CsElement(const tinyxml2::XMLElement& e)  : elem_( &e ) { }

    //bool valid_ = true;

    const tinyxml2::XMLElement* elem_;

};

////////////////////////////////////////////////////////////////////////////////
//  reading XML.

inline CsElement read(const tinyxml2::XMLElement& elem)
{
    // OK, we have expanded our Cs-path into a non-null
    // element 'elem'. now let the user work on the 
    // returned CsElement instead.
    return CsElement( elem );

}


// FIXME:
/*
// C :: std::string =>
template <typename... Cs>
CsElement read(const tinyxml2::XMLElement& elem, const std::string& c, const Cs&... cs)
{

    return read( elem, c.c_str(), cs... );

}
*/

// C :: const char* =>
template <typename C, typename... Cs>
CsElement read(const tinyxml2::XMLElement& elem, const C& c, const Cs&... cs)
{
    // find child child with name 'c'
    auto* child = elem.FirstChildElement( c );
    if ( child )
    {
        // continue expande our Cs-path
        return read( *child, cs... );
    }

    // no such child found
    return CsElement();
}


template <typename... Cs>
CsElement read(tinyxml2::XMLDocument& doc, const Cs&... cs)
{

    // note that a XML document, i.e. a XML file, requires 
    // one and only one parent element (???).
    // this element is not ignored in our Cs-path (???)

    auto* child = doc.RootElement();
    if ( child )
    {
        return read( *child, cs... );
    }

    // no root element found for XML document
    return CsElement();

}


////////////////////////////////////////////////////////////////////////////////

inline bool read_values(const tinyxml2::XMLElement* elem)
{
    return true;
}

template <typename T, typename... Ts>
bool read_values(const tinyxml2::XMLElement* elem, T& t, Ts&... ts)
{
    // assert we have an element to read from
    if ( elem == nullptr )
    {
        return false; 
    }

    // assert this element has a value
    const char* c_str = elem->GetText();
    if ( c_str == nullptr )
    {
        return false;
    }

    // assert we retrieved a value 'tmp' for this element
    T tmp;
    std::string str = c_str;
    std::istringstream is( str );
    if ( !(is >> tmp) )
    {
        return false;
    }
    
    // continue and with tail, assert success
    if ( !read_values( elem->NextSiblingElement(), ts... ) )
    {
        return false; 
    }

    // T and Ts are OK, return success
    t = tmp;
    return true;

// unsafe (undefined values upon failure):
/*
    // check if tail is OK (Haskell foldl):
    if ( read_values( elem->NextSiblingElement(), ts... ) )
    {
        // elem has a value (XMLText), try to read into 't':
        const char* c_str = elem->GetText();
        if ( c_str )
        {
            std::string str = c_str;
            std::istringstream is( str );
            if ( is >> t )
            {
                return true;
            }
        }

    }

    return false;
*/
}

////////////////////////////////////////////////////////////////////////////////
// read single value (XMLText of element)
template <typename T, typename... Ts>
bool read_value_empty(const tinyxml2::XMLElement& elem, T& t, Ts&... ts)
{

    return false;
}

inline bool read_value_empty(const tinyxml2::XMLElement& elem)
{

    return true;
}

template <typename T, typename... Ts>
bool read_value(const tinyxml2::XMLElement& elem, T& t, Ts&... ts)
{
    // verify that Ts... is empty:
    // FIXME: probably a std function for this 
    if ( read_value_empty( elem, ts... ) )
    {
        // if there is a value (XMLText), try to read it into 't':
        const char* c_str = elem.GetText();
        if ( c_str )
        {
            std::string str = c_str;
            std::istringstream is( str );
            if ( is >> t )
            {
                return true;
            }

        }
    }

    return false;


}



inline bool CsElement::operator()()
{
    return elem_;
}

template <typename T, typename... Ts>
bool CsElement::operator()(T& t, Ts&... ts)
{
    // this CsElement is either valid or non-valid
    // (elem_ == nullptr)
    //
    // lets see if this child just contains a value (XMLText)
    // (and hence, typically, no children).
    //
    // otherwise, consider the children of this child as a set S.
    // S may be empty, since this child has no children.
    // for each element s in S, s should have a value (XMLText), 
    // which we map to the respective T in Ts.

    if ( elem_ )
    {
        if ( elem_->GetText() )
        {
            // work on the element 'elem_'
            return read_value( *elem_, t, ts... );
        }
        else
        {
            // work on children of the element 'elem_'
            return read_values( elem_->FirstChildElement(), t, ts... );
        }
        
    }
    return false;
}

// the content of [b, e) is undefined upon failure, i.e. false return.
template <typename OutputIterator>
bool CsElement::operator()(OutputIterator b, OutputIterator e)
{
    using namespace tinyxml2;
    if ( elem_ )
    {
        // we assume a list of values (working on the children of 'elem_')
        if ( elem_->GetText() == nullptr )
        {
            const XMLElement* child = elem_->FirstChildElement();
          
            // fill up [b, e) each at a time
            while ( b != e )
            {
                // 
                if ( child == nullptr ) return false;

                // assure child has a value (XMLText)
                const char* c_str = child->GetText();
                if ( c_str == nullptr ) return false;

                // elem has a value (XMLText), try to read into '*b':
                std::string str = c_str;
                std::istringstream is( str );
                is >> *b;
                if ( is.fail() )
                {
                    return false;
                }
                
                // continue...
                child = child->NextSiblingElement();
                ++b;
            }

            return true;
        }

    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace xml

} // namespace batb


#endif
