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
#ifndef BATB_DEMO_COURSECURVE_HPP
#define BATB_DEMO_COURSECURVE_HPP
#include "batb/batb_include.hpp"
#include "batb/forest.hpp"
#include "batb/forest/World.hpp"
#include "batb/event.hpp"
#include <vector>


namespace batb
{


namespace demo
{

class CourseCurve
{
public:
    using Point = glm::vec2;
    CourseCurve();

    void create(const Point&, const Point& );

    Point operator()(float_t );
    Point operator()(uint , uint );

private:
    std::vector<Point> points_;


};



} // namespace demo

} // namespace batb




#endif
