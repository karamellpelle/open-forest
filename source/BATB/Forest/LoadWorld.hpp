//    open-forest: an orientering game.
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
#ifndef BATB_FOREST_LOADWORLD_HPP
#define BATB_FOREST_LOADWORLD_HPP
#include "include.hpp"



namespace batb
{
class BATB;

namespace forest
{
class World;




// setup or destroy a World
class LoadWorld
{
public:
    LoadWorld(BATB* );

    void load(const YAML::Node& , World* );
    void unload(World& );


private:
    void shadows(World* );
    void load(const YAML::Node& , Terrain* );

    BATB* batb = nullptr;

};


} // namespace forest

} // namespace batb




#endif
