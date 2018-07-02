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
#include "BATB/Forest/MapViewer.hpp"
#include "BATB/Forest.hpp"
#include "BATB/Forest/World.hpp"
#include "BATB/Forest/Terrain.hpp"
#include "BATB/Scene.hpp"


namespace batb
{

namespace forest
{

MapViewer::MapViewer() 
{

}

void MapViewer::mapscale(uint one, uint many)
{
    scale_  = (double)( one ) / (double)( many );
    scale_inv_ = (double)( many ) / (double)( one );
}

void MapViewer::setPosition(double x, double z, tick_t ticks)
{
    p0_ = x;
    p1_ = z;
}

void MapViewer::setDirection(double x, double z, tick_t ticks)
{
    auto mm = x * x + z * z;
    auto ku = mm == 0 ? 1.0 : 1.0 / sqrt( mm );
    u0_ = ku * x;
    u1_ = ku * z;
    v0_ = -u1_;
    v1_ = u0_;
}

void MapViewer::lookAt(double x, double z, tick_t ticks)
{

    setDirection( x - p0_ , z - p1_ );

}


void MapViewer::setRotation(double rad, tick_t ticks)
{
    double ux, uy;
    cossin( rad, ux, uy );
    setDirection( ux, uy );
}

void MapViewer::setZoom(double a, tick_t ticks)
{
    zoom_ = a;
    zoom_inv_ = (1.0) / a;
}

//void MapViewer::useMap(Map* m)
//{
//    map = m; 
//}
// we want to work on the FBO target with meter coordinates to make the 
// map as real as possible.
//

//void MapViewer::init(World* forest)
//{
//    // since we work directly with references to World, we can't initialize again.
//    if ( forest_ ) throw std::runtime_error( "init() non-empty MapViewer" );
//
//
//    // always work on this World
//    forest_ = forest;
//
//    
//}

void MapViewer::draw2D(BATB* batb, const Scene& scene)
{
    ////////////////////////////////////////////////////////////////
    // this draws in 2D

    auto nvg = batb->gl->nanovgBegin( scene );
    nvgSave( nvg );

    // define size of NanoVG context, in pixels and meters
    // we need to work with pixel since NanoVG has problem with 
    // non-pixel coordinates, especially fonts

    // set origo in the middle 
    nvgTranslate( nvg, 0.5 * scene.wth_px, 0.5 * scene.hth_px );

    // meter context -> pixel of context
    double from_m = scene.wth_px / scene.wth_m;

    Draw2D draw2d;
    draw2d.from_m = from_m * zoom_;
    draw2d.to_m = scale_;
    draw2d.to_pixel = draw2d.from_m * draw2d.to_m;

    // the context defines a world region of this size
    // also make sure we take care of zooming
    double world_wth = scene.wth_m * scale_inv_ * zoom_inv_;
    double world_hth = scene.hth_m * scale_inv_ * zoom_inv_;

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

    ////////////////////////////////////////////////////////////////
    //
   
    // begin 2D drawing
    nvgSave( nvg );
    beginDraw2D( nvg, draw2d );
    nvgRestore( nvg );

    // do 2D drawing
    //nvgSave( nvg );
    //if ( map ) map->draw2D( nvg, draw2d );
    //nvgRestore( nvg );

    // end 2D drawing
    nvgSave( nvg );
    endDraw2D( nvg, draw2d );
    nvgRestore( nvg );



    nvgRestore( nvg );
    batb->gl->nanovgEnd(); 
}


void MapViewer::step(tick_t tick)
{
    // make sure we can handle very fast framerate (tick == tick_)
    if ( tick_ < tick )
    {

        tick_ = tick;
    }
}


} // namespace forest

} // namespace batb



