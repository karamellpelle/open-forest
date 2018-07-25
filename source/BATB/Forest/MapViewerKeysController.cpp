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
    auto* mapview = keys->mapview;
    auto* mapview_plus = keys->mapview_plus;
    auto* mapview_shift = keys->mapview_shift;
    auto* mapview_ctrl = keys->mapview_ctrl;

    // if there is a change of Key press, restart with current values
    if ( mapview->changed() || mapview_plus->changed() || mapview_shift->changed() || mapview_ctrl->changed() )
    {
        // save current MapViewer view state
        mapviewer_->getPosition( p0x_, p0z_ );
        mapviewer_->getRotation( rotate0_ );
        mapviewer_->getZoom( zoom0_ );
        mapviewer_->getOpacity( opacity0_ );

        // save current pointer
        keys->aim->axis( pointer_x0_, pointer_y0_ );
    }

    if ( mapview->press() )
    {
        active_ = true;

        // retrieve current ponter drag
        float_t pointer_x1, pointer_y1;
        keys->aim->axis( pointer_x1, pointer_y1 );
        auto pointer_x = pointer_x1 - pointer_x0_;
        auto pointer_y = pointer_y1 - pointer_y0_;

        // key: ctrl
        if ( mapview_ctrl->press() )
        {
            // key: shift
            if ( mapview_shift->press() )
            {
                // key: plus
                if ( mapview_plus->press() )
                {
                    // opacity
                    auto opacity1 = opacity0_ + (-0.1) * pointer_y;
                    mapviewer_->setOpacity( opacity1 );

                    goto mapview_finished;
                }


                // TODO: rotate fine
                auto rotate1 = rotate0_ + (-1.0) * pointer_y;
                mapviewer_->setRotation( rotate1 );
            }
            goto mapview_finished;
        }

        // key: plus
        if ( mapview_plus->press() )
        {
            // key: shift
            if ( mapview_shift->press() )
            {
                double zoom1 = std::max( 0.0, zoom0_ + (double)( -10 * pointer_y ) );
                mapviewer_->setZoom( zoom1 );
                goto mapview_finished;
            }


            // move
            //float_t delta_x = -20000.0 * pointer_x;
            //float_t delta_z = -20000.0 * pointer_y;
            //
            //glm::vec3 position = p0_ + glm::vec3( delta_x, 0.0, delta_z );
            double x = p0x_ + (3.0) * pointer_x;
            double z = p0z_ + (3.0) * pointer_y;
            mapviewer_->setPosition( x, z );

            goto mapview_finished;
        }

        // key: shift
        if ( mapview_shift->press() )
        {
            // rotate
            auto rotate1 = rotate0_ + (-16.0) * pointer_y;
            mapviewer_->setRotation( rotate1 );
        }
    }
    else
    {
        active_ = false;
    }
    
    mapview_finished:

    dragging_ = mapview->press() && ( mapview_plus->press() || mapview_shift->press() );

    // physics step dt
    //auto tick = batb->time->get();
    //if ( tick_ < tick )
    //{
    //
    //    tick_ = tick;
    //}

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



