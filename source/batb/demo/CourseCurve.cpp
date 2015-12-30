//    open-demo: an orientering game.
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
#include "batb/demo/CourseCurve.hpp" 
#include "batb.hpp" 

namespace batb
{



namespace demo
{


CourseCurve::CourseCurve()
{

}

// create random path p0 -> p1
void CourseCurve::create(const Point& p0, const Point& p1)
{
    static std::default_random_engine rand; 
    

    // number of control points
    constexpr uint between_max = 8;
    uint between = std::uniform_int_distribution<uint>( 0, between_max )( rand );

    points_.clear();

    // first point: p0
    points_.push_back( p0 );

    // control points
    Point d = p1 - p0; 
    Point dn = Point( -d.y, d.x );
    for (uint i = 0; i != between; ++i)
    {
        // reference on p0 -> p1
        //constexpr float r_step = 0.2;
        //float div = 1.0 / (float)( 2 + between );
        //std::uniform_real_distribution<float_t> gen_step( -r_step, r_step );
        //float step = ( 1.0 + (float)( i ) + gen_step( rand ) ) * div;
        float step = std::uniform_real_distribution<float_t>( 0.0, 1.0 )( rand );

        // distance on normal from reference
        constexpr float r_stepn = 0.5;
        float stepn = std::uniform_real_distribution<float_t>( -r_stepn, r_stepn )( rand );

        // add a random control point between p0 and p1
        points_.push_back( glm::mix( p0, p1, step ) + stepn * dn );

    }

    // last point: p1
    points_.push_back( p1 );
}


CourseCurve::Point CourseCurve::operator()(float_t alpha)
{
    // here, we assume size is non-empty

    // copy (important; points_ must not be modified)
    auto points = points_;

    uint n = points.size();
    while ( n != 0 )
    {
        for (uint i = 0; i + 1 != n; ++i)
        {
            // interpolate p0 -> p1
            points[ i ] *= (float)((1.0 - alpha) );
            points[ i ] += (float)( alpha ) * points[ i + 1 ];
        }
        --n;
    }

    return points[0];
}


CourseCurve::Point CourseCurve::operator()(uint m, uint i)
{
    return operator()( (float_t)( i ) / (float_t)( m ) );
}


} // namespace demo

} // namespace batb


