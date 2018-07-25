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
#ifndef BATB_FOREST_MAPVIEWER_HPP
#define BATB_FOREST_MAPVIEWER_HPP
#include "BATB/Forest.hpp"
#include "BATB/GL.hpp"



namespace batb
{
class BATB;
class Scene;

namespace forest
{
class World;
class MapDrawer2D;





// TODO: work on dedicated FBO/texture and blit to Scene
class MapViewer 
{
public:
    MapViewer();

    // connect to world
    //void init(World* );

    void step(tick_t );
    void draw2D(BATB* , const Scene& );

    ////////////////////////////////////////////////////////////////
    //
    //// use this map
    //void useMap(Map* );
    // mapscale alá orienteering map
    void mapscale(uint one, uint many); // map_drawer.mapscale( 1, 15000 );

    // position, in MapViewer coordinates 
    // (which happes to be, at least for now, Shape coordinates)
    void setPosition(double x, double y, tick_t = 0.0);
    void getPosition(double& , double& ) const;

    // rotation
    void setRotation(double , tick_t = 0.0); 
    void getRotation(double& ) const; 
    void setDirection(double x, double y, tick_t = 0.0); // upward direction
    void lookAt(double x, double y, tick_t = 0.0);

    // zoom
    void setZoom(double , tick_t = 0.0); // 1.0 is mapscale
    void getZoom(double& ) const; 

    // opacity
    void setOpacity(double );
    void getOpacity(double& ) const;



    ////////////////////////////////////////////////////////////////
    // view map based by World coordinates. 
    // TODO: use decltype of world coordinates
    void setPositionWorld(const glm::vec3& , tick_t = 0.0);
    void getPositionWorld(glm::vec3& ) const;
    // look direction, i.e. upwards
    void lookAtWorld(const glm::vec3& , tick_t = 0.0);

    ////////////////////////////////////////////////////////////////
    // we need to work with pixel since NanoVG has problem with 
    // non-pixel coordinates, especially fonts
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

        double opacity = 1.0;
    };

protected:
    virtual void beginDraw2D(NVGcontext* , const Draw2D& ) {}
    virtual void endDraw2D(NVGcontext* , const Draw2D& )   {}


private:
    tick_t tick_ = 0.0;

    double scale_     = 1.0;
    double scale_inv_ = 1.0;


    double p0x_ = 0.0;
    double p0z_ = 0.0;
    double p1x_ = 0.0;
    double p1z_ = 0.0;
    tick_t p_tick_ = 0.0;

    double rotate0_ = 0.0;
    double rotate1_ = 0.0;
    tick_t rotate_tick_ = 0.0;

    double opacity0_ = 1.0;
    double opacity1_ = 1.0;
    tick_t opacity_tick_ = 0.0;

    double zoom0_     = 1.0;
    double zoom1_     = 1.0;
    tick_t zoom_tick_ = 0.0;


};


} // namespace forest

} // namespace batb


#endif

