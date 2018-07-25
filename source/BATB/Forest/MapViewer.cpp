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

#include "BATB/CourseDrawer.hpp"

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

void MapViewer::setPosition(double x, double y, tick_t ticks)
{
    p0x_ = x;
    p0z_ = y;
}


void MapViewer::setRotation(double rad, tick_t ticks)
{
    //rad = std::fmod( rad, twopi );
    rotate0_ = rad;
}

void MapViewer::setDirection(double x, double y, tick_t ticks)
{
    auto rotate = std::atan2( x, y );
    setRotation( rotate, ticks);
}

void MapViewer::lookAt(double x, double y, tick_t ticks)
{

    setDirection( x - p0x_, y - p0z_, ticks);

}



void MapViewer::setZoom(double a, tick_t ticks)
{
    zoom0_ = a;
}

void MapViewer::setOpacity(double a)
{
    opacity0_ = clamp( a, 0.0, 1.0 );
}

void MapViewer::getZoom(double& z) const
{
    z = zoom0_;
}


void MapViewer::getPosition(double& x, double& y) const
{
    x = p0x_;
    y = p0z_;
}

void MapViewer::getRotation(double& r) const
{
    r = rotate0_;
}


void MapViewer::getOpacity(double& o) const
{
    o = opacity0_;
}

void MapViewer::setPositionWorld(const glm::vec3& p, tick_t ticks)
{
    auto px = p.x;
    auto py = p.z;
    // TODO: convert!
    //setPosition( px, py, ticks );
}

void MapViewer::lookAtWorld(const glm::vec3& p, tick_t ticks)
{
    
    setDirection( p.x - p0x_, p.z - p0z_ );

}




void MapViewer::draw2D(BATB* batb, const Scene& scene)
{
    double px, pz;
    getPosition( px, pz );
    double rotate;
    getRotation( rotate );
    double zoom;
    getZoom( zoom );
    double opacity;
    getOpacity( opacity );


    ////////////////////////////////////////////////////////////////
    // this draws in 2D

    auto nvg = batb->gl->nanovgBegin( scene );
    nvgSave( nvg );

    // define size of NanoVG context, in pixels and meters
    // we need to work with pixel since NanoVG has problem with 
    // non-pixel coordinates, especially fonts

    // set origo in the middle 
    nvgTranslate( nvg, 0.5 * scene.wth_px, 0.5 * scene.hth_px );
#if 0
    auto glfw_window_ = batb->screen->glfw_window;
    int wth; int hth;
    glfwGetWindowSize( glfw_window_, &wth, &hth );
    double x_, y_;
    glfwGetCursorPos( glfw_window_, &x_, &y_ );
    nvgStrokeColor(nvg, nvgRGBA(255,255,0, 255));
    nvgFillColor(nvg, nvgRGBA(255,255,0,255));
    nvgBeginPath( nvg );
    nvgArc( nvg, x_, y_, 20.0, 0, twopi, NVG_CCW );
    nvgFill( nvg );
#endif
#if 1
    float_t x,y;
    batb->forest->keys->aim->axis( x, y );
    auto to_pixel = [&](float_t m) { return m * std::max( scene.wth_px, scene.hth_px ); };

    nvgStrokeColor(nvg, nvgRGBA(0,255,255, 255));
    nvgFillColor(nvg, nvgRGBA(0,255,128,255));
    nvgBeginPath( nvg );
    nvgArc( nvg, to_pixel( x ) + 40, to_pixel( y ), 15.0, 0, twopi, NVG_CCW );
    nvgFill( nvg );
#endif

    // meter context -> pixel of context
    double from_m = scene.wth_px / scene.wth_m;

    Draw2D draw2d;
    draw2d.from_m = from_m * zoom; // meter on map to pixels
    draw2d.to_m = scale_;           // meter in Terrain to meter on map
    draw2d.to_pixel = draw2d.from_m * draw2d.to_m; // meter in Terrain to pixels
    draw2d.opacity = opacity;

    ////////////////////////////////////////////////////////////////
    // define World region to draw 
    // FIXME: don't know if this is correct

    // MapViewer 
    // the Scene context defines a world region of this size
    //double world_wth = scene.wth_m * scale_inv_ * zoom_inv_;
    //double world_hth = scene.hth_m * scale_inv_ * zoom_inv_;
    double world_wth = scene.wth_m / (scale_ * zoom);
    double world_hth = scene.hth_m / (scale_ * zoom);

    // find bounding box in World 
    double ux, uz;
    cossin( rotate, ux, uz );
    double vx = uz;
    double vz = -ux;
    double a0 = world_hth * ux;
    double a1 = world_hth * uz;
    double b0 = world_wth * vx;
    double b1 = world_wth * vz;
    double min_x = std::min( a0 , b0 );
    double max_x = std::max( a0 , b0 );
    double min_z = std::min( a1 , b1 );
    double max_z = std::max( a1 , b1 );

    // MapViewer position to World position
    double world_x = (px * std::max( scene.wth_m, scene.hth_m ) ) / (scale_ * zoom);
    double world_z = (pz * std::max( scene.wth_m, scene.hth_m ) ) / (scale_ * zoom);
    draw2d.x = (world_x + min_x) - 0.5 * world_hth; // from center to corner
    draw2d.z = (world_z + min_z) - 0.5 * world_wth; // from center to corner
    draw2d.w = draw2d.x + max_x;
    draw2d.h = draw2d.z + max_z;


    // translate and rotate (world is scaled down to pixel level)
    //auto p0 = draw2d.to_pixel * p0_.x;
    //auto p1 = draw2d.to_pixel * p0_.z;
    //nvgTransform( nvg, u0_.x, u0_.z, v0_.x, v0_.z, 0.0, 0.0 );
    //nvgTranslate( nvg, -p0, -p1 );
    nvgRotate( nvg, rotate );
   
    // MapViewer position is in Shape coordinates, hence convert
    // to pixels
    auto px_x = px * std::max( scene.wth_px, scene.hth_px );
    auto px_z = pz * std::max( scene.wth_px, scene.hth_px );
    nvgTranslate( nvg, -px_x, -px_z );

    {
        CourseDrawer drawer;
        drawer.numbers( true );
        // unit size in drawing (defined by radius of a Normal control), in world coordinates
        drawer.size( 16 ); // FIXME: more customizable settings

        drawer.begin( nvg );
        drawer.start( glm::vec2( 0.0, 0.0 ) );
        drawer.normal( glm::vec2( 100.0, 0.0 ) );
        drawer.normal( glm::vec2( 250.0, 100.0 ) );
        drawer.end();

        drawer.begin( nvg );
        drawer.finish( glm::vec2( 0.0, 0.0 ) );
        drawer.normal( glm::vec2( 0.0, 80.0 ) );
        drawer.normal( glm::vec2( 0.0, 160.0 ) );
        drawer.normal( glm::vec2( 0.0, 290.0 ) );
        drawer.end();
    }
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



