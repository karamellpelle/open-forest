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
#ifndef BATB_FOREST_MAPDRAWER2D_HPP
#define BATB_FOREST_MAPDRAWER2D_HPP
#include "BATB/GL.hpp"
#include "BATB/Forest/Map.hpp"


namespace batb
{
namespace forest
{

class World;
class Map;


// draw a Map to a 2D NanovgContext
// Map in meters (world coordinates), NanoVG context in millimeter coordinates
class MapDrawer2D
{
public:
    MapDrawer2D();
   
    // use this map
    void useMap(Map* );
    // mapscale alá orienteering map
    void mapscale(uint one, uint many); // map_drawer.mapscale( 1, 15000 );

   
    ////////////////////////////////////////////////////////////////
    // view map based by World coordinates. 
    // world coordinates makes it easy to switch Map's on the fly
    // TODO: use decltype of world coordinates

    // view draw map from this center
    void setPosition(double x, double z);
    // up direction
    void setDirection(double u0, double u1);
    void lookAt(double x, double y);
    void setRotation(double ); // [0, twopi)

    // working on context
    void setZoom(double ); // 1.0 is mapscale

    double getScaling() const;

    // we need this since NanoVG has problem with non-pixel
    // coordinates
    class Draw
    {
    public:
        double wth = 0.0; // width of nanovg target, in pixels
        double hth = 0.0; // height of nanovg target, in pixels
        double wth_m = 0.0; // width of nanovg target, in pixels
        double hth_m = 0.0; // height of nanovg target, in pixels
        //double from_m = 1.0; // scaling meter -> pixel

    };

    // draw to coordinate system of given nanovg context
    // MapDrawer2D assumes the unit of the nanovg context is 1.0 meter
    void draw(NVGcontext* nvg, const Draw& );

protected:
    // this does no scaling
    // identitiy:
    //    world x/north -> -y (screen up)
    //    world z/east  -> x (screen right)
    // (i.e. north is up)

    double p0_ = 0.0;
    double p1_ = 0.0;
    double u0_ = 1.0;
    double u1_ = 0.0;
    double v0_ = 0.0;
    double v1_ = 1.0;
   

    virtual void beginMapDraw(NVGcontext* , const Map::Draw2D& ) {}
    virtual void endMapDraw(NVGcontext* , const Map::Draw2D& )   {}

    double scale_     = 1.0;
    double scale_inv_ = 1.0;


    Map* map         = nullptr;
    double zoom_     = 1.0;
    double zoom_inv_ = 1.0;

};




} // namespace forest

} // namespace batb



#endif
