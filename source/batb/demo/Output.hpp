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
#ifndef BATB_DEMO_OUTPUT_HPP
#define BATB_DEMO_OUTPUT_HPP
#include "batb/batb_include.hpp"
#include "game/Output.hpp"

namespace batb
{


class BATB;


namespace demo
{
class World;


// the default outputter (OpenGL, sound, network, ...)
class Output : public game::Output<World>
{
public:
    Output(BATB& b);

    virtual void operator()(World& ) override;

    // TODO: controls:
    // ...

protected:
    void update(World& );
    void aim(World& );

    BATB& batb;

    World* world_ = nullptr;

    uint course_i_ = 0;
    tick_t course_tick_ = 0.0;
    glm::vec2 course_u0_;
    glm::vec2 course_u1_;
    glm::vec2 course_p0_;
    glm::vec2 course_p1_;
    tick_t course_tick_d_ = 0.0;
    float_t course_d0_ = 1.0;
    float_t course_d1_ = 1.0;

    float_t course_size_ = 1.0; 
};


} // namespace demo

} // namespace batb


#endif

