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
#ifndef BATB_CONFIG_HPP
#define BATB_CONFIG_HPP
#include <string>
#include <stdexcept>
#include "BATB/BATB_include.hpp"
#include "BATB/Values.hpp"



namespace BATB
{


// Config is actually just a XMLElement, for now
class Config : public xml::XMLDocument
{
public:
    Config(const std::string& path) : path_( path )
    {
        using namespace xml;
        XMLError err = LoadFile( path.c_str() );
        switch ( err )
        {
        case XML_ERROR_FILE_NOT_FOUND: throw std::runtime_error( "could not create BATB::Config (XML_ERROR_FILE_NOT_FOUND)" );
        case XML_ERROR_FILE_COULD_NOT_BE_OPENED: throw std::runtime_error( "could not create BATB::Config (XML_ERROR_FILE_COULD_NOT_BE_OPENED)" );  
        case XML_ERROR_FILE_READ_ERROR: throw std::runtime_error( "could not create BATB::Config (XML_ERROR_FILE_READ_ERROR)" );
        }
    }


private:
    const std::string path_;
};

}


#endif
