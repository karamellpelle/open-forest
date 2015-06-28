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
#ifndef BATB_FOREST_ITERATION_ITERATION_FOREST_DEMO_HPP
#define BATB_FOREST_ITERATION_ITERATION_FOREST_DEMO_HPP
#include "batb/forest/iteration/IterationForest.hpp"


namespace batb
{


class BATB;

namespace forest
{




// IterationForestDemo:
//
class IterationForestDemo : public IterationForest
{
friend void begin(IterationForestDemo& );
friend void end(IterationForestDemo& );

public:
    // construct from the containing BATB
    IterationForestDemo(BATB& );

    // iterate
    void iterate_forest(IterationStack& stack, World& world) override;

    // setup before iteration
    void iterate_begin(World& );

    ////////////////////////////////////////

private:

    float_t aim_a = 0.0;  // yaw
    float_t aim_b = 0.0;  // pitch
    float_t aim_c = 0.0;  // roll

    Runner* aiming = nullptr;

    tick_t aim_tick = 0.0;
};

////////////////////////////////////////////////////////////////////////////////
//  

void begin(IterationForestDemo& );


void end(IterationForestDemo& );



} // namespace forest

} // namespace batb

#endif
