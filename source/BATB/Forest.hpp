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
#ifndef BATB_FOREST_HPP
#define BATB_FOREST_HPP
#include "BATB/ModuleBATB.hpp"
#include "BATB/Forest/KeySet.hpp"




namespace batb
{


namespace forest
{




class Forest : public ModuleBATB
{
public:
    Forest(BATB* b);

    void begin(const std::string& );
    void end();


    std::unique_ptr<KeySet> keys;
    std::unique_ptr<EventList> events;

private:


};



} // namespace forest

} // namespace batb


#endif

