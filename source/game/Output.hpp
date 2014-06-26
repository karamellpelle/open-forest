//    orienteering-game: an orientering game.
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
#ifndef GAME_OUTPUT_HPP
#define GAME_OUTPUT_HPP
#include "Ref.hpp"

namespace Game
{


template <typename A>
class Output : public Ref
{
friend class Iteration<A>;
public:
    virtual ~Output() { }   // FIXME: protected??

    // we prefer named constructors and destructors, so for 
    // subclass ObjectX, make static functions:
    //
    // static OutputX* create(Y y)          { ... }
    // static void     destroy(Output<A>* ) { ... }

    virtual void outputWorld(A& a) = 0;

};


}

#endif
