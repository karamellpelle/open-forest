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
#include "File.hpp"

namespace File
{

// full path to read-only application data
// example: image, 
std::string staticData(const std::string& path)
{
    // TMP:
    return "data/static/" + path;
}

// full path to read-write application data
// example: configuration file, 
std::string dynamicData(const std::string& path)
{
    // TMP:
    return "data/dynamic/" + path;
}

// full path to user files (reachable outside application)
// example: screenshot, 
std::string user(const std::string& path)
{
    // TMP:
    return "data/user/" + path;
}

// full path to temporary files (files that can be removed after application)
// example: log file, 
std::string tmp(const std::string& path)
{
    // TMP:
    return "data/tmp/" + path;
}

}

