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
#include "BATB/Forest.hpp"
#include "BATB/Forest/MapDrawer2D.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Terrain.hpp"


namespace batb
{

namespace forest
{


MapDrawer2D::MapDrawer2D() 
{

}

void MapDrawer2D::mapscale(uint one, uint many)
{
    scale_  = (double)( one ) / (double)( many );
    scale_inv_ = (double)( many ) / (double)( one );
}

void MapDrawer2D::setPosition(double x, double y)
{
    p0_ = x;
    p1_ = y;
}

void MapDrawer2D::setDirection(double x, double y)
{
    auto mm = x * x + y * y;
    auto ku = mm == 0 ? 1.0 : 1.0 / sqrt( mm );
    u0_ = ku * x;
    u1_ = ku * y;
    v0_ = -u1_;
    v1_ = u0_;
}

void MapDrawer2D::lookAt(double x, double y)
{

    setDirection( x - p0_ , y - p1_ );

}


void MapDrawer2D::setRotation(double rad)
{
    double ux, uy;
    cossin( rad, ux, uy );
    setDirection( ux, uy );
}

void MapDrawer2D::setZoom(double z)
{
    zoom_ = z;
    zoom_inv_ = (1.0) / z;
}

void MapDrawer2D::useMap(Map* m)
{
    map = m; 
}

void MapDrawer2D::draw(NVGcontext* nvg, const Draw& draw)
{
    // meter context -> pixel of context
    double from_m = draw.wth / draw.wth_m;

    Map::Draw2D draw2d;
    draw2d.from_m = from_m * zoom_;
    draw2d.to_m = scale_;
    draw2d.to_pixel = draw2d.from_m * draw2d.to_m;

    // the context defines a world region of this size
    // also make sure we take care of zooming
    double world_wth = draw.wth_m * scale_inv_ * zoom_inv_;
    double world_hth = draw.hth_m * scale_inv_ * zoom_inv_;

    // find bounding box (world coordinates)
    // FIXME: verify correctness
    double a0 = world_hth * u0_;
    double a1 = world_hth * u1_;
    double b0 = world_wth * v0_;
    double b1 = world_wth * v1_;
    double min_x = std::min( a0 , b0 );
    double max_x = std::max( a0 , b0 );
    double min_z = std::min( a1 , b1 );
    double max_z = std::max( a1 , b1 );

    draw2d.x = (p0_ + min_x) - 0.5 * world_hth; // from center to corner
    draw2d.z = (p1_ + min_z) - 0.5 * world_wth; // from center to corner
    draw2d.w = draw2d.x + max_x;
    draw2d.h = draw2d.z + max_z;


    // translate and rotate (world is scaled down to pixel level)
    auto p0 = draw2d.to_pixel * p0_;
    auto p1 = draw2d.to_pixel * p1_;
    nvgTransform( nvg, v0_, -u0_, v1_, -u1_, -p0, -p1 );

// debug: draw axis
#if 0
    nvgStrokeWidth( nvg, 4.0 ); 
    // x
    nvgStrokeColor(nvg, nvgRGBA(255,0.0,0.0,255));
    nvgBeginPath( nvg );
    nvgMoveTo( nvg, 0.0, 0.0 );
    nvgLineTo( nvg, 100 , 0.0 ); 
    nvgStroke( nvg );
    // z
    nvgStrokeColor(nvg, nvgRGBA(0.0,0.0,255,255));
    nvgBeginPath( nvg );
    nvgMoveTo( nvg,  0.0 ,  0.0  );
    nvgLineTo( nvg,  0.0 ,  100 ); 
    nvgStroke( nvg );
#endif
   
    nvgSave( nvg );
    beginMapDraw( nvg, draw2d );
    nvgRestore( nvg );

    nvgSave( nvg );
    if ( map ) map->draw2D( nvg, draw2d );
    nvgRestore( nvg );

    nvgSave( nvg );
    endMapDraw( nvg, draw2d );
    nvgRestore( nvg );
}


} // namespace forest

} // namespace batb



