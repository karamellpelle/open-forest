//    open-forest: an orientering game.
//    Copyright (C) 2014  carljsv@student.matnat.uio.no
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
#ifndef BATB_COURSEDRAWER_HPP
#define BATB_COURSEDRAWER_HPP
#include "batb/batb_include.hpp"

namespace batb
{

class BATB;

// render course using World coordinates
class CourseDrawer
{
public:
    CourseDrawer(BATB& batb);

    enum class ObjectType { Empty, Normal, Start, Finish, };
   

    // unit size (radius of Normal) defined in World units
    void size(float_t r)            { scale_size_ = r; }

    // draw indices?
    void numbers(bool i)            { indices_ = i; }


    // new drawing
    // assuming inside Nanovg rendering
    void begin();
    void end();

    // current color
    void color(float_t , float_t , float_t , float_t = 1.0);


    // xy-plane
    void normal(const glm::vec2& );
    void start(const glm::vec2& );
    void finish(const glm::vec2& );

    // xz-plane
    void normal(const glm::vec3& p)   { normal( glm::vec2( p.x, p.z ) ); }
    void start(const glm::vec3& p)    { start( glm::vec2( p.x, p.z ) ); }
    void finish(const glm::vec3& p)   { finish( glm::vec2( p.x, p.z ) ); }

private:
    BATB& batb;

public:
    bool draw(const glm::mat3& , ObjectType );        // draw only this
    void push_draw(const glm::vec3& p, ObjectType );  // draw previuos, this next
    float_t size(ObjectType );

    NVGcontext* nvg = nullptr;
    
    glm::vec3 p0; // previous point - 1
    glm::vec3 p1; // previous point 

    ObjectType type0; // type of previos point - 1
    ObjectType type1; // type of previos point

    // color
    NVGcolor nanovg_color_next_;
    bool color_next_ = false;
    

    float_t scale_size_ = 1.0;

    // numbering
    bool indices_ = true;
    uint ix_;
    int nanovg_font_ = -1;

    // sizes (size_normal_ should be reference (i.e. 1))
    float_t size_normal_;   
    float_t size_start_;
    float_t size_finish_a_;
    float_t size_finish_b_;
    float_t size_ix_;
    float_t size_ix_pad_;
    float_t size_line_;
};


} // namespace batb

#endif

