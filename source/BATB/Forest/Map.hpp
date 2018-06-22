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
#ifndef BATB_FOREST_MAP_HPP
#define BATB_FOREST_MAP_HPP
#include "BATB/GL.hpp"


namespace batb
{

namespace forest
{
class World;
class Terrain;



// a 2D view of Terrain
class Map
{
public:
    // implies a Terrain
    Terrain* terrain = nullptr;

    //////////////////////////////////////////////////////////////////////////////// 
    // draw map
    class Draw2D
    {
    public:
        // scaling: world -> context
        //double scale = 1.0;
        //double scale_inv = 1.0;

        // bounding box, world coordinates
        double x = 0.0;
        double z = 0.0;
        double w = 1.0;
        double h = 1.0;
    
        // world coordinates scaled down (i.e. mapscale)
        double to_m = 1.0;
        // mapscale to NanoVG coordinates
        double from_m = 1.0;
        // world coordinates to NanoVG coordinates (NanoVG fonts needs pixels)
        double to_pixel = from_m * to_m;
    };
    // draw region defined by Draw2D
    virtual void draw2D(NVGcontext* nvg, const Draw2D& draw) { }

    // defining how Map is mapped into Terrain
    // ...
    
    std::string name;

private:
    //GLuint tex = 0;
    //GLuint pos,xxx
    //uint tex_wth_;
    //uint tex_hth_;
};



} // namespace forest

} // namespace batb



#endif

