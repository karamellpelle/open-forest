//    open-demo: an orientering game.
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
#ifndef BATB_DEMO_COURSE_HPP
#define BATB_DEMO_COURSE_HPP
#include "include.hpp"
#include "BATB/Forest/World.hpp"


namespace batb
{

namespace forest { class World; }


namespace demo
{

class Course
{
public:
    Course();

    void clear();

    void init(forest::World* f) { forest_ = f; }

    void addControl(float_t, float_t, forest::ControlDefinition::Code, forest::ControlDefinition::Type);
    forest::Control* operator[](uint ) const;
   
    uint size() const               { return controls_.size(); }
    bool empty() const              { return controls_.empty(); }

    // dimensions
    float_t radius(float_t , float_t ) const;
    float_t width() const;
    float_t width(float_t ) const;
    float_t height() const;
    float_t height(float_t ) const;
    float_t dimension() const;
    float_t dimension(float_t, float_t ) const;
    glm::vec2 center() const;

private:
    forest::World* forest_;
    std::vector<forest::Control*> controls_;

    float_t x_min_ = 0.0;
    float_t x_max_ = 0.0; 
    float_t z_min_ = 0.0; 
    float_t z_max_ = 0.0;
    
};


} // namespace demo

} // namespace batb




#endif
