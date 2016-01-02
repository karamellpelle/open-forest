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
#include "batb/demo/Course.hpp" 
#include "batb/forest/World.hpp" 

namespace batb
{



namespace demo
{

Course::Course(forest::World& w) : forest_( w )
{

}


void Course::addControl(float_t x, float_t z, forest::ControlDefinition::Code c, forest::ControlDefinition::Type t)
{
    if ( controls_.empty() )
    {
        x_min_ = x, x_max_ = x;
        z_min_ = z, z_max_ = z;
    }

    // add a _new_ control to forest::World
    forest::ControlDefinition def( x, z, c, t);
    controls_.push_back( forest_.addControl( def ) );

    x_min_ = std::min( x_min_, x );
    x_max_ = std::max( x_max_, x );
    z_min_ = std::min( z_min_, z );
    z_max_ = std::max( z_max_, z );
}


forest::Control* Course::operator[](uint ix) const
{
    if ( controls_.size() <= ix ) return nullptr;

    return controls_[ix];
}


void Course::clear()
{
    controls_.clear();

    x_min_ = 0.0;
    x_max_ = 0.0; 
    z_min_ = 0.0; 
    z_max_ = 0.0;

    // (added controls are not removed)
}

float_t Course::dimension() const
{
    return std::max( x_max_ - x_min_, z_max_ - z_min_ );
}

float_t Course::dimension(float_t x, float_t z) const
{
    auto xm = 2.0 * std::max( std::abs( x_max_ - x ), std::abs( x_min_ - x ) );
    auto zm = 2.0 * std::max( std::abs( z_max_ - z ), std::abs( z_min_ - z ) );
    
    return std::max( xm, zm );
}

glm::vec2 Course::center() const
{
    return glm::vec2( 0.5 * (x_max_ + x_min_), 0.5 * (z_max_ + z_min_) );
}

} // namespace demo

} // namespace batb


