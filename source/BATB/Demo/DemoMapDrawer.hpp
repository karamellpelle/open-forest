//    open-demo: an orientering game.
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
#ifndef BATB_DEMO_DEMOMAPDRAWER_HPP
#define BATB_DEMO_DEMOMAPDRAWER_HPP
#include "include.hpp"
#include "BATB/Forest/MapDrawer2D.hpp"


namespace batb
{
namespace demo
{
class World;


// draw a Map to a 2D NanovgContext
class DemoMapDrawer : public forest::MapDrawer2D
{
public:
    DemoMapDrawer();
    
    void init(World* );

protected:
    World* demo_ = nullptr;

    void beginMapDraw(NVGcontext* , const forest::Map::Draw2D&) override;
    void endMapDraw(NVGcontext* , const forest::Map::Draw2D& ) override;


};




} // namespace demo

} // namespace batb



#endif
