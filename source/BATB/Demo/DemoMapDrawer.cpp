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
#include "BATB/Demo/DemoMapDrawer.hpp"
#include "BATB/Demo/World.hpp"
#include "BATB/CourseDrawer.hpp"


namespace batb
{

namespace demo
{


DemoMapDrawer::DemoMapDrawer()
{

}

void DemoMapDrawer::init(World* d)
{
    demo_ = d;
}


void DemoMapDrawer::beginMapDraw(NVGcontext* nvg, const forest::Map::Draw2D& region)
{
  
}

// draw region defines part of world to be drawn.
// draw region of world is in world coordinates.
// target NanoVG coordinates is pixels
void DemoMapDrawer::endMapDraw(NVGcontext* nvg, const forest::Map::Draw2D& draw)
{
    using namespace forest;
    
    auto from_m = [&](float_t m) { return (float_t)( draw.from_m * m ); };
    auto to_pixel = [&](float_t m) { return (float_t)( draw.to_pixel * m ); };

    // IOF reference is mapscale of 1:15000
    // according to IOF rules (http://orienteering.org/wp-content/uploads/2017/04/ISOM2017_20May2017.pdf), the refernence 
    // map scale is 1 : 15000. for other scales, all symbols shall be scaled proportionally, even control rings and lines.
    // see part 3.7 in document above.
    //
    // mm of 1:15000 map -> pixel
    auto from_mm15000 = [&](float_t mm) { return from_m( draw.to_m * (mm * 15.0) ); };


    // (assuming area nonempty)
#if 0
    nvgStrokeWidth( nvg, from_m( 0.01 ) ); // 1 cm
    // x
    nvgStrokeColor(nvg, nvgRGBA(255,0.0,0.0,255));
    nvgBeginPath( nvg );
    nvgMoveTo( nvg, from_m( 0.0 ), from_m( 0.0 ) );
    nvgLineTo( nvg, from_m( 0.04 ), from_m( 0.0 ) ); 
    nvgStroke( nvg );
    // z
    nvgStrokeColor(nvg, nvgRGBA(0.0,0.0,255,255));
    nvgBeginPath( nvg );
    nvgMoveTo( nvg, from_m( 0.0 ), from_m( 0.0 ) );
    nvgLineTo( nvg, from_m( 0.0 ), from_m( 0.04 ) ); 
    nvgStroke( nvg );
#endif

#if 1
    // these works on world coordinates, hence scaled down
    auto moveTo = [&](const TracePoint& p) { nvgMoveTo( nvg, to_pixel( p.x ), to_pixel( p.z ) ); };
    auto lineTo = [&](const TracePoint& p) { nvgLineTo( nvg, to_pixel( p.x ), to_pixel( p.z ) ); };
    //auto mm15000 = [&](float_t mm) { return (mm * 15.0) * region.scale; };

    auto& trace = demo_->runner->trace;
    auto& run = *(demo_->run);
    auto& pos = demo_->runner->move.pos;
    auto& course = demo_->course;
    
    

    nvgStrokeWidth( nvg, from_mm15000( 0.5 ) );
    nvgLineCap(nvg, NVG_ROUND );
    nvgStrokeColor(nvg, nvgRGBA(0,192,255,128));

    const auto& points = trace.points;
    if ( !points.empty() )
    {
        nvgBeginPath(nvg);
        //// only show the newest trace
        //constexpr uint tails = 32; 
        //uint b = tails <= points.size() ? points.size() - tails : 0;
        uint b = 0;
        uint e = points.size();
        moveTo( points[b] );
        for (uint i = b + 1; i != points.size(); ++i)
        {
            lineTo( points[i] );
        }
        lineTo( trace.point0 );
        lineTo( TracePoint( demo_->forest->tick, glm::vec3( pos ) ) );

        nvgStroke(nvg);
        
    }

    // draw current position
    nvgStrokeColor(nvg, nvgRGBA(255,0,0,255));
    nvgFillColor(nvg, nvgRGBA(255,0,0,255));
    nvgBeginPath( nvg );
    nvgArc( nvg, to_pixel( pos.x ), to_pixel( pos.z ), from_mm15000( 0.5 ), 0, twopi, NVG_CCW );
    nvgFill( nvg );
    
    ////////////////////////////////////////////////////////////////////////////////
    // draw course
#endif
#if 0
    if ( 2 <= course.size() )
    {
        CourseDrawer drawer;
        drawer.numbers( true );
        // unit size in drawing (defined by radius of a Normal control), in world coordinates
        drawer.size( from_mm15000( 2.5 ) ); // FIXME: more customizable settings

        drawer.begin( nvg );
        for (uint i = 0; i != course.size(); ++i)
        {
            forest::Control* control = course[i];
            auto pos = glm::vec2( to_pixel( control->aim.pos.x ), to_pixel( control->aim.pos.z ) );

            switch ( control->definition.type )
            {
            case forest::ControlDefinition::Type::Start:
                drawer.start( pos );
            break;
            case forest::ControlDefinition::Type::Normal:
                drawer.normal( pos );
            break;
            case forest::ControlDefinition::Type::Finish:
                drawer.finish( pos );
            break;
            }


        }
        drawer.end();
    }
#endif
#if 1
        CourseDrawer drawer;
        drawer.numbers( true );
        // unit size in drawing (defined by radius of a Normal control), in world coordinates
        drawer.size( 15 ); // FIXME: more customizable settings

            
            drawer.begin( nvg );
            drawer.start( glm::vec2( 0.0, 0.0 ));
            drawer.normal( glm::vec2( 75, 0.0 ));
            drawer.start( glm::vec2( 150, 0.0 ));
            drawer.end();

            drawer.begin( nvg );
            drawer.finish( glm::vec2( 0.0, 0.0 ));
            drawer.normal( glm::vec2( 0.0, 75 ));
            drawer.finish( glm::vec2( 0.0, 150 ));
            drawer.end();
#endif
}

} // namespace demo

} // namespace batb



