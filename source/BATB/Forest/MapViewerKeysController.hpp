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
#ifndef BATB_FOREST_MAPVIEWERKEYSCONTROLLER_HPP
#define BATB_FOREST_MAPVIEWERKEYSCONTROLLER_HPP
#include "BATB/Forest.hpp"



namespace batb
{
class BATB;

namespace forest
{
class World;
class MapViewer;





class MapViewerKeysController 
{
public:
    MapViewerKeysController();

    // connect to world
    void connect(MapViewer* );

    void step(BATB* );

    bool isActive() const;
    bool isDragging() const;

private:
    MapViewer* mapviewer_ = nullptr;

    tick_t tick_ = 0.0;

    float_t pointer_x0_ = 0.0;
    float_t pointer_y0_ = 0.0;
    bool active_ = false;
    bool dragging_ = false;

    glm::vec3 p0_;
    float_t rotate0_ = 0.0;

    float_t transparency0_ = 1.0;

};


} // namespace forest

} // namespace batb


#endif

