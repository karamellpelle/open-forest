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
    u0_.x = 1.0;
    u0_.z = 0.0;
    v0_.x = 0.0;
    v0_.z = 1.0;
}

void MapViewer::mapscale(uint one, uint many)
{
    scale_  = (double)( one ) / (double)( many );
    scale_inv_ = (double)( many ) / (double)( one );
}

void MapViewer::setPosition(const glm::vec3& p, tick_t ticks)
{
    //p0_ = x;
    //p1_ = z;

    p0_ = p;
}

void MapViewer::setDirection(const glm::vec3& d, tick_t ticks)
{
    auto mm = d.x * d.x + d.z * d.z;
    auto ku = mm == 0 ? 1.0 : 1.0 / sqrt( mm );
    u0_.x = ku * d.x;
    u0_.z = ku * d.z;
    v0_.x = -u0_.z;
    v0_.z = u0_.x;

    //rotate0_ = std::atan2( u0_.x, -u0_.y );
}

void MapViewer::lookAt(const glm::vec3& p, tick_t ticks)
{

    //setDirection( x - p0_ , z - p1_ );
    setDirection( p - p0_ );

}


void MapViewer::setRotation(double rad, tick_t ticks)
{
    rad = std::fmod( rad, twopi );

    double ux, uz;
    cossin( rad, ux, uz );
    setDirection( glm::vec3( ux, 0.0, uz ) );
}

void MapViewer::setZoom(double a, tick_t ticks)
{
    zoom_ = a;
    zoom_inv_ = (1.0) / a;
}

void MapViewer::setOpacity(double a)
{
    opacity0_ = clamp( a, 0.0, 1.0 );
}

double MapViewer::getZoom() const
{
    return zoom_;
}


glm::vec3 MapViewer::getPosition() const
{
    return p0_;
}

glm::vec3 MapViewer::getDirection() const
{
    return u0_;
}
double MapViewer::getRotation() const
{
    return rotate0_;
}
double MapViewer::getOpacity() const
{
    return opacity0_;
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
    draw2d.opacity = opacity0_;

    // the context defines a world region of this size
    // also make sure we take care of zooming
    double world_wth = scene.wth_m * scale_inv_ * zoom_inv_;
    double world_hth = scene.hth_m * scale_inv_ * zoom_inv_;

    // find bounding box (world coordinates)
    // FIXME: verify correctness
    double a0 = world_hth * u0_.x;
    double a1 = world_hth * u0_.z;
    double b0 = world_wth * v0_.x;
    double b1 = world_wth * v0_.z;
    double min_x = std::min( a0 , b0 );
    double max_x = std::max( a0 , b0 );
    double min_z = std::min( a1 , b1 );
    double max_z = std::max( a1 , b1 );

    draw2d.x = (p0_.x + min_x) - 0.5 * world_hth; // from center to corner
    draw2d.z = (p0_.z + min_z) - 0.5 * world_wth; // from center to corner
    draw2d.w = draw2d.x + max_x;
    draw2d.h = draw2d.z + max_z;


    // translate and rotate (world is scaled down to pixel level)
    auto p0 = draw2d.to_pixel * p0_.x;
    auto p1 = draw2d.to_pixel * p0_.z;
    nvgTransform( nvg, u0_.x, u0_.z, v0_.x, v0_.z, 0.0, 0.0 );
    nvgTranslate( nvg, -p0, -p1 );

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



