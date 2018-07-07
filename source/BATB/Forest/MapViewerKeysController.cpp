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
#include "BATB/Forest/MapViewer.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Time.hpp"


namespace batb
{

namespace forest
{

MapViewerKeysController::MapViewerKeysController() 
{

}

void MapViewerKeysController::connect(MapViewer* mv)
{
    // always work on this World
    mapviewer_ = mv;

    
}

void MapViewerKeysController::step(BATB* batb)
{
    // make sure we can handle very fast framerate (tick == tick_)
    auto& keys = batb->forest->keys;
    bool pressed = keys->mapview->pressed()         ||
                   keys->mapview_plus->pressed()    ||
                   keys->mapview_shift->pressed()  ||
                   keys->mapview_ctrl->pressed();
    // if there is a change of Key press, restart with current values
    if ( pressed )
    {
        // save current Keys state
        p0_ = mapviewer_->getPosition();
        rotate0_ = mapviewer_->getRotation();

        keys->aim->axis( pointer_x0_, pointer_y0_ );
    }

    if ( keys->mapview->press() )
    {
        // retrieve current ponter position
        float_t pointer_x1, pointer_y1;
        keys->aim->axis( pointer_x1, pointer_y1 );

        if ( keys->mapview_plus->press() )
        {
            float_t delta_x = -10000.0 * (pointer_x1 - pointer_x0_);
            float_t delta_z = -10000.0 * (pointer_y1 - pointer_y0_);

            glm::vec3 position = p0_ + glm::vec3( delta_x, 0.0, delta_z );
            mapviewer_->setPosition( position );
        }

        active_ = true;
    }
    else
    {
        active_ = false;
    }

    dragging_ = keys->mapview->press() && keys->mapview_plus->press();

    // physics step dt
    auto tick = batb->time->get();
    if ( tick_ < tick )
    {

        tick_ = tick;
    }
}

bool MapViewerKeysController::isDragging() const
{
    return dragging_;
}

bool MapViewerKeysController::isActive() const
{
    return active_;
}


} // namespace forest

} // namespace batb



