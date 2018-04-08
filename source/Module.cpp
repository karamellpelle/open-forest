//    open-forest: an orienteering game.
//    Copyright (C) 2018  karamellpelle@hotmail.com
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
#include <fstream>
#include "Module.hpp"

void Module::config(const YAML::Node& cfg)
{
    // important: YAML::Node is similar to a shared_ptr
    yaml = cfg;     
    filepath_.clear();
}

void Module::config(const std::string& path)
{
    try
    {
        filepath_ = path;
        yaml = YAML::LoadFile( filepath_ );
    }
    catch (std::exception& e)
    {
        std::ostringstream os;
        os << "Module could not load YAML: " << path;
        throw std::runtime_error( os.str() );
    }
}


void Module::save()
{
    // save iff this Module has its own config file
    if ( !filepath_.empty() )
    {
        save( filepath_ );
    }
}

void Module::save(const std::string& path)
{
    std::ofstream os( path );
    os << yaml;
}


