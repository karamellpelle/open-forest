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
#include "batb/xml/XML.hpp"


namespace batb
{

namespace xml
{

void begin(XML& xml)
{

    xml.initialized_ = true;
}

void end(XML& xml)
{
    if ( xml.initialized_ )
    {

    }
    xml.initialized_ = false;
}

////////////////////////////////////////////////////////////////////////////////
//  

tinyxml2::XMLError load_document(tinyxml2::XMLDocument& doc, const std::string& filepath, const std::string& tag, std::string& errstr)
{
    using namespace tinyxml2;

    XMLError err = doc.LoadFile( filepath.c_str() );
    if ( err == XML_ERROR_FILE_NOT_FOUND )
    {
        std::ostringstream os;
        os << tag << ": file not found, " + filepath + ". (XML_ERROR_FILE_NOT_FOUND)";
        errstr = os.str();

        return err;
    }
    if ( err == XML_ERROR_FILE_COULD_NOT_BE_OPENED )
    {
        std::ostringstream os;
        os << tag << ": file could not be opened. (XML_ERROR_FILE_COULD_NOT_BE_OPENED)";
        errstr = os.str();

        return err;
    }
    if ( err == XML_ERROR_FILE_READ_ERROR )
    {
        std::ostringstream os;
        os << tag << ": file could not be read.  (XML_ERROR_FILE_READ_ERROR)";
        errstr = os.str();

        return err;
    }
    if ( err == XML_ERROR_EMPTY_DOCUMENT )
    {
        std::ostringstream os;
        os << tag << ": file is empty. (XML_ERROR_EMPTY_DOCUMENT)";
        errstr = os.str();

        return err;
    }
    if ( err != XML_NO_ERROR )
    {
        const char* str1 = doc.GetErrorStr1();
        const char* str2 = doc.GetErrorStr2();
        std::ostringstream os;
        os << tag << ": file parsing error";
        if ( str1 ) os << ": " << str1;
        if ( str2 ) os << ", " << str2;
        errstr = os.str();

        return err;
    }

    return XML_NO_ERROR;
}


tinyxml2::XMLError save_document(tinyxml2::XMLDocument& doc, const std::string& filepath, const std::string& tag, std::string& errstr)
{
    using namespace tinyxml2;
    XMLError err = doc.SaveFile( filepath.c_str() );
    
    if ( err != XML_NO_ERROR )
    {
        const char* str1 = doc.GetErrorStr1();
        const char* str2 = doc.GetErrorStr2();
        std::ostringstream os;
        os << THIS_FUNCTION << ": save error"; 
        if ( str1 ) os << ": " << str1;
        if ( str2 ) os << ", " << str2;
       
        errstr = os.str();

    }
    return err;
}


} // namespace xml

} // namespace batb
