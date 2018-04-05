//    open-forest: an orienteering game.
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
#include "batb.hpp"
#include "batb/CourseDrawer.hpp"
#include "batb/value/batb.hpp"

namespace batb
{



CourseDrawer::CourseDrawer(BATB& b) : batb( b )
{

    // set values 
    size_normal_    = value::coursedrawerSizeNormal;
    size_start_     = value::coursedrawerSizeStart;
    size_finish_a_  = value::coursedrawerSizeFinishA;
    size_finish_b_  = value::coursedrawerSizeFinishB;
    size_ix_        = value::coursedrawerSizeIx;
    size_ix_pad_    = value::coursedrawerSizeIxPad;
    size_line_      = value::coursedrawerSizeLine;

    /// set nanovg context to work on
    nvg = batb.gl.nvg_context;

    // set default color
    nanovg_color_next_ = nvgRGBAf( 
                         value::coursedrawerColorR,
                         value::coursedrawerColorG,
                         value::coursedrawerColorB,
                         value::coursedrawerColorA );
    
    // find font
    nanovg_font_ = nvgFindFont( nvg, "CourseDrawer" );
    if ( nanovg_font_ == -1 )
    {
        // create font
        nanovg_font_ = batb.gl.nanovg_font( "CourseDrawer", 
                       file::static_data( "batb/CourseDrawer.ttf" ) );
    }


}

void CourseDrawer::begin()
{
    type0 = ObjectType::Empty; 
    p0 = glm::vec3(-1.0, 0.0, 1.0);
    type1 = ObjectType::Empty; 
    p1 = glm::vec3(0.0, 0.0, 1.0);
    ix_ = 0;

    // convert line width to pixels
    float_t w = scale_size_ * size_line_;
    nvgStrokeWidth( nvg, w );


    // line end and join
    nvgLineCap( nvg, NVG_ROUND );   // necessary?
    nvgLineJoin( nvg, NVG_ROUND );

    // size of index text
    nvgFontSize( nvg, scale_size_ * size_ix_ );
    nvgTextAlign( nvg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE );

    // set color's
    nvgStrokeColor( nvg, nanovg_color_next_ );
    nvgFillColor( nvg, nanovg_color_next_ );
    
}


void CourseDrawer::end()
{
    // draw last object (object 1)

    // just continue direction. or easier: u = glm::vec2( 1, 0 );
    auto u = glm::normalize( p1 - p0 );

    glm::mat3 trans(  u.x, u.y, 0,
                     -u.y, u.x, 0,
                      p1.x, p1.y, 1);

    draw( trans, type1 );

}

void CourseDrawer::color(float_t r, float_t g, float_t b, float_t a)
{
    nanovg_color_next_ = nvgRGBAf( r, g, b, a );
    color_next_ = true;
}

void CourseDrawer::push_draw(const glm::vec3& p2, ObjectType type2)
{
    auto dir = p2 - p1;
    float_t len = glm::length( dir );
    auto u = (float)(1.0 / len) * dir; // unit direction

    // draw from p0 in direction of p1
    glm::mat3 trans(  u.x, u.y, 0,
                     -u.y, u.x, 0,
                      p1.x, p1.y, 1);
  
    // draw object 1, if not empty
    if ( draw( trans, type1 ) )
    { 
        // line from object 1 to object 2
        auto l1 = trans * glm::vec3( size( type1 ), 0, 1.0 );
        auto l2 = trans * glm::vec3( len - size( type2 ), 0, 1.0 );
        nvgBeginPath( nvg );
        nvgMoveTo( nvg, l1.x, l1.y );
        nvgLineTo( nvg, l2.x, l2.y );
        nvgStroke( nvg );


        // draw numbers?
        if ( indices_ )
        {
            // only for normal's
            if ( type1 == ObjectType::Normal )
            {
                auto v = glm::normalize( p0 - p1 );
                auto m = (float)(-0.5) * (u + v);

                if ( auto len = glm::length( m ) )
                {
                    m = (float)(1.0 / len) * m;
                }
                else
                {
                    m = glm::vec3( 1.0, 0.0, 0.0 );
                }
                
                auto a = (float)(size_ix_pad_ * scale_size_) * m;
                a.z = 1.0;
                auto x = trans[2] + a;
                
                // draw text
                std::ostringstream os;
                os << ix_;
                nvgFontFaceId( nvg, nanovg_font_ );
                nvgText( nvg, x.x, x.y, os.str().c_str(), nullptr );
                
            }

        }
        
        // drawable objects have indices
        ++ix_;

    }

    // set color for next object 
    if ( color_next_ )
    {
        nvgStrokeColor( nvg, nanovg_color_next_ );
        nvgFillColor( nvg, nanovg_color_next_ );

        color_next_ = false;
    }

    // push object 2 as next
    p0 = p1;
    type0 = type1;
    p1 = p2;
    type1 = type2;

    
}


void CourseDrawer::normal(const glm::vec2& p2)
{
    push_draw( glm::vec3( p2, 1.0 ), ObjectType::Normal );  
}

void CourseDrawer::start(const glm::vec2& p2)
{
    push_draw( glm::vec3( p2, 1.0 ), ObjectType::Start );  
}

void CourseDrawer::finish(const glm::vec2& p2)
{
    push_draw( glm::vec3( p2, 1.0 ), ObjectType::Finish );  

    // TODO: reset ix_, set Empty?
}

float_t CourseDrawer::size(ObjectType type)
{
    switch ( type )
    {
    case ObjectType::Empty:   return 0.0 * scale_size_;
    case ObjectType::Normal:  return size_normal_ * scale_size_;
    case ObjectType::Start:   return size_start_ * scale_size_; 
    case ObjectType::Finish:  return size_finish_b_ * scale_size_;
    } 
    return 0.0;
}

// draw type at given point and direction 
// return true iff something was actually drawn ( if line to next object )
bool CourseDrawer::draw(const glm::mat3& trans, ObjectType type)
{
    if ( type == ObjectType::Normal )
    {
        auto a = size_normal_ * scale_size_;
        auto x = trans * glm::vec3( 0.0, 0.0, 1.0 );

        // draw circle
        nvgBeginPath( nvg );
        nvgArc( nvg, x.x, x.y, a, 0, twopi, NVG_CCW );
        nvgStroke( nvg );
        return true;
    }
    if ( type == ObjectType::Start )
    {
        constexpr float_t start_x0 = 1.0;
        constexpr float_t start_x1 = -0.5;
        constexpr float_t start_y0 = 0.8660254037844386;
        constexpr float_t start_y1 = -0.8660254037844386;
        auto s = size_start_ * scale_size_;
        auto a = trans * glm::vec3( s * start_x0, 0.0, 1.0 );
        auto b = trans * glm::vec3( s * start_x1, s * start_y0, 1.0 );
        auto c = trans * glm::vec3( s * start_x1, s * start_y1, 1.0 );
        // draw triangle
        nvgBeginPath( nvg );
        nvgMoveTo( nvg, a.x, a.y );
        nvgLineTo( nvg, b.x, b.y );
        nvgLineTo( nvg, c.x, c.y );
        nvgClosePath( nvg );
        nvgStroke( nvg );
        return true;
    }
    if ( type == ObjectType::Finish )
    {
        auto a = size_finish_a_ * scale_size_;
        auto b = size_finish_b_ * scale_size_;
        auto x = trans * glm::vec3( 0.0, 0.0, 1.0 );  

        // draw two circles
        nvgBeginPath( nvg );
        nvgArc( nvg, x.x, x.y, a, 0, twopi, NVG_CCW );
        nvgStroke( nvg );
        nvgBeginPath( nvg );
        nvgArc( nvg, x.x, x.y, b, 0, twopi, NVG_CCW );
        nvgStroke( nvg );
        return true;
    }

    return false; // i.e. type == ObjectType::Empty
}


} // namespace batb



