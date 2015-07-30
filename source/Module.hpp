//    open-forest: an orienteering game.
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
#ifndef MODULE_HPP
#define MODULE_HPP
#include "include.hpp"
#include "yaml-cpp/yaml.h"

class Module
{
public:
    Module() : initialized_( false ) { }

    void config(const YAML::Node& );    // from given Node
    void config(const std::string& );   // from Node read from file
    void save();                        // if Node was read from file, write back
    void save(const std::string& );     // save to file

protected:
    YAML::Node yaml;
    std::string filepath;

    bool init_empty() const { return !initialized_; }
    bool init_nonempty() const { return initialized_; }
    void init(bool b) { initialized_ = b; }

private:
    bool initialized_;

};


#endif

