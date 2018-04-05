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
#ifndef BATB_RUN_WORKERS_HPP
#define BATB_RUN_WORKERS_HPP
#include "batb/batb_include.hpp"
#include "batb/run/iteration/IterationRunWork.hpp"


namespace batb
{



namespace run
{

////////////////////////////////////////////////////////////////////////////////
// load non-core part of BATB
template <>
class LoadWorker<BATB>
{
public:
    LoadWorker(BATB& b) : batb( b ) { }
    void operator()(Work& );

    BATB& batb;
};


  
////////////////////////////////////////////////////////////////////////////////
// unload non-core part of BATB
template <>
class UnloadWorker<BATB>
{
public:
    UnloadWorker(BATB& b) : batb( b ) { }
    void operator()(Work& );

    BATB& batb;
};


  
} // namespace run

} // namespace batb

#endif
