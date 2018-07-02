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
#include "BATB/Forest/MapViewerKeysController.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Time.hpp"


namespace batb
{

namespace forest
{

MapViewerKeysController::MapViewerKeysController() 
{

}

void MapViewerKeysController::init(World* forest)
{
    // since we work directly with references to World, we can't initialize again.
    if ( forest_ ) throw std::runtime_error( "init() non-empty MapViewerKeysController" );


    // always work on this World
    forest_ = forest;

    
}

void MapViewerKeysController::step(BATB* batb)
{
    // make sure we can handle very fast framerate (tick == tick_)
    auto tick = batb->time->get();
    while ( tick_ < tick )
    {

        tick_ = tick;
    }
}

bool MapViewerKeysController::isDragging()
{

}


} // namespace forest

} // namespace batb



