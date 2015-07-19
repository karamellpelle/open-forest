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
#ifndef BATB_FOREST_FOREST_HPP
#define BATB_FOREST_FOREST_HPP
#include "batb/batb_include.hpp"
#include "batb/ModuleBATB.hpp"
#include "batb/forest/KeySet.hpp"
#include "batb/forest/iteration/IterationForestDemo.hpp"




namespace batb
{


class BATB;


namespace forest
{




class Forest : public ModuleBATB
{
friend void begin(Forest& );
friend void end(Forest& );

public:
    Forest(BATB& b);

    KeySet keyset;

    // Iteration's
    IterationForestDemo iterationForestDemo;

private:


};


void begin(Forest& );
void end(Forest& );

} // namespace forest

} // namespace batb


#endif

