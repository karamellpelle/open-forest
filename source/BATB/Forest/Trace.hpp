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
#ifndef BATB_FOREST_TRACE_HPP
#define BATB_FOREST_TRACE_HPP
#include "include.hpp"


namespace batb
{
namespace forest
{


// point in a trace
class TracePoint
{
public:
    TracePoint() { }
    TracePoint(tick_t t, const glm::vec3& v) : tick( t ), x( v[0] ), y( v[1] ), z( v[2] )  { }
      
    tick_t tick = 0.0;
  
    // use 'float' to save mem?
    float_t x;
    float_t y;
    float_t z;

    // TODO: other information like intensity, fatigue, etc

};


// line of TracePoint's
class Trace
{
public:
    void push(const TracePoint& point1)
    {
        points.push_back( point0 );
        point0 = point1;
    }

    void pushIf(float_t epseps, const TracePoint& point1)
    {
        float_t x = point1.x - point0.x;
        float_t y = point1.y - point0.y;
        float_t z = point1.z - point0.z;
        if ( epseps <= x * x + y * y + z * z )
        {
            push( point1 );
        }
    }
    void position(const TracePoint& p)
    {
        point0 = p;
    }
    void reset(const TracePoint& p)
    {
        points.clear();
        point0 = p;
    }

    using TracePoints = std::vector<TracePoint>;
    uint size() const { return points.size(); }

    TracePoints points;
    TracePoint point0;

private:
};


} // namespace forest

} // namespace batb



#endif
