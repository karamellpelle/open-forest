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
#ifndef BATB_DEMO_DEMO_HPP
#define BATB_DEMO_DEMO_HPP
#include "batb/batb_include.hpp"
#include "batb/ModuleBATB.hpp"
#include "batb/demo/KeySet.hpp"
#include "batb/demo/iteration/IterationDemoDemo.hpp"




namespace batb
{


class BATB;


namespace demo
{




class Demo : public ModuleBATB
{
friend void begin(Demo& );
friend void end(Demo& );

public:
    Demo(BATB& b);

    KeySet keyset;

    // Iteration's
    IterationDemoDemo iterationDemoDemo;

private:


};


void begin(Demo& );
void end(Demo& );

} // namespace demo

} // namespace batb


#endif

